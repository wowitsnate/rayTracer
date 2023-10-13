#include "../Math/Vector.h"
#include <cstdint>
#include "../Colour/Colour.h"
#include "../Objects/Hittable.h"
#include "../PPM/PPM.h"
#include "Camera.h"

#include <future>

#include "../Utility/Utility.h"
#include <iostream>
#include <thread>

#include "../Materials/Material.h"

#define NUMBER_OF_THREADS 200

Camera::Camera(const CameraInitData& data)
{
	//Positions
	m_lookFrom = data.lookFrom;
	m_lookAt = data.lookAt;
	m_vUp = data.vUp;

	m_vfov = data.vFov;

	m_imageWidth = data.imageWidth;
	m_imageHeight = static_cast<int>(static_cast<int>(data.imageWidth) / data.aspectRatio);
	m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

	m_cameraCenter = m_lookFrom;

	//viewport Dimensions
	//m_focalLength = (m_lookFrom - m_lookAt).magnitude();

	m_focusDist = data.focusDist;
	m_defocusAngle = data.defocusAng;

	auto theta = degrees_to_radians(m_vfov);
	auto H = std::tan(theta / 2);
	m_viewportHeight = 2 * H * m_focusDist;
	m_viewportWidth = m_viewportHeight * (static_cast<double>(m_imageWidth) / m_imageHeight);

	//u, v, w vectors
	w = (m_lookFrom - m_lookAt).normalize();
	u = m_vUp.cross(w).normalize();
	v = w.cross(u);

	//Horizontal and vertical viewport edges
	m_viewportU = u * m_viewportWidth;
	m_viewportV = (-v) * m_viewportHeight;

	//horizontal and vertical delta's
	m_pixelDeltaU = m_viewportU / m_imageWidth;
	m_pixelDeltaV = m_viewportV / m_imageHeight;

	m_viewportUpperLeft = m_cameraCenter - (w * m_focusDist) - (m_viewportU / 2) - (m_viewportV / 2);
	m_pixel00Loc = m_viewportUpperLeft + ((m_pixelDeltaU + m_pixelDeltaV) * 0.5);

	//camera defocus vectors
	double defocusRadius = m_focusDist * std::tan(degrees_to_radians(m_defocusAngle / 2.0));
	m_defocusDiskU = u * defocusRadius;
	m_defocusDiskV = v * defocusRadius;

	//extra stuff
	m_samplesPerPixel = data.samplesPerPixel;
	m_maxRayBounces = data.maxRayBounces;
}

/*
void Camera::render(const std::unique_ptr<HittableList>& HittableList, PPM& ppmOut)
{

	for (auto j = 0; j < ppmOut.getHeight(); ++j) {

		std::clog << "\rScanlines Percent Done: " << (int)(((double)(j) / (double)ppmOut.getHeight()) * 100.0) << "% " << std::flush;

		for (auto i = 0; i < ppmOut.getWidth(); ++i) {

			Colour PixelColour(0.0, 0.0, 0.0);

			for (int sample = 0; sample < m_samplesPerPixel; ++sample) {
				Ray r = getRay(i, j);
				PixelColour += this->getRayColour(r, m_maxRayBounces, HittableList);
			}

			ppmOut.m_data.push_back(PixelColour);
		}
	}
}*/

struct thread_info_t
{
	int i;
	int j;
	int sample_amount;
	int max_ray_bounces;
	const std::unique_ptr<HittableList>& hitList;
	Camera* pThis;
};

Colour RayColourThread(const thread_info_t& info)
{
	auto res = Colour(0.0 ,0.0, 0.0);

	for (int sample_count = 0; sample_count < info.sample_amount; ++sample_count)
	{
		Ray r = info.pThis->getRay(info.i, info.j);
		res += info.pThis->getRayColour(r, info.max_ray_bounces, info.hitList);
	}

	return res;
}


std::vector<int> split_evenly(int total, int workers) {
	std::vector<int> split = {};

	const auto [quotient, remainder] = std::div(total, workers);

	for (int i = 0; i < remainder; i++)
	{
		split.push_back(quotient + 1);
	}

	for (int i = 0; i < (workers - remainder); i++)
	{
		split.push_back(quotient);
	}

	return split;
}


void Camera::render(const std::unique_ptr<HittableList>& HittableList, PPM& ppmOut)
{

	for (auto j = 0; j < ppmOut.getHeight(); ++j) {

		std::clog << "\rScanlines Percent Done: " << (int)(((double)(j) / (double)ppmOut.getHeight()) * 100.0) << "% " << std::flush;

		for (auto i = 0; i < ppmOut.getWidth(); ++i) {

			std::vector<std::future<Colour>> worker_asyncs;
			std::vector<int> worker_counts = split_evenly(m_samplesPerPixel, NUMBER_OF_THREADS);

			for (int z = 0; z < NUMBER_OF_THREADS; z++)
			{
				thread_info_t info{
					i,
					j,
					worker_counts[z],
					m_maxRayBounces,
					HittableList,
					this
				};

				auto a = std::async(&RayColourThread, info);
				worker_asyncs.push_back(std::move(a));
			}

			Colour PixelColour = Colour(0.0, 0.0, 0.0);
			for (auto & a : worker_asyncs)
			{
				a.wait();
				PixelColour += a.get();
			}


			ppmOut.m_data.push_back(PixelColour);
		}
	}
}





Vector3 Camera::getPixelCenter(const int i, const int j) const
{
	return m_pixel00Loc + (m_pixelDeltaU * i) + (m_pixelDeltaV * j);
}

Colour Camera::getRayColour(const Ray& r, int depth, const std::unique_ptr<HittableList>& HittableList)
{
	HitRecord rec;

	if (depth < 0)
	{
		return { 0.0, 0.0, 0.0 };
	}

	if (HittableList->intersects(r, Interval(0.001, std::numeric_limits<double>::infinity()), rec)) {

		Ray scattered;
		Colour attenuation;


		if (rec.mat->scatter(r, rec, attenuation, scattered))
			return getRayColour(scattered, depth - 1, HittableList) * attenuation;

		return Colour(0.0, 0.0, 0.0);
	}

	const Vector3 unitDir = r.direction.normalize();
	const auto a = 0.5 * (unitDir.y + 1.0);
	const auto res = Vector3::lerp({ 0.5, 0.7, 1.0 }, { 1.0, 1.0, 1.0 }, a);

	return { res.x, res.y, res.z };
}

Ray Camera::getRay(int i, int j)
{

	auto pixel_center = m_pixel00Loc + (m_pixelDeltaU * i) + (m_pixelDeltaV * j);
	auto pixel_sample = pixel_center + pixelSampleSquare();

	auto ray_origin = (m_defocusAngle <= 0) ? m_cameraCenter : defocusDiskSample(); //Doouble check this
	auto ray_direction = pixel_sample - ray_origin;

	return Ray(ray_origin, ray_direction);
}

Vector3 Camera::pixelSampleSquare() const
{
	auto px = -0.5 + genRandomDouble();
	auto py = -0.5 + genRandomDouble();
	return (m_pixelDeltaU * px) + (m_pixelDeltaV * py);
}

Point3 Camera::defocusDiskSample() const
{
	auto p = random_in_unit_disk();
	return m_cameraCenter + (m_defocusDiskU * p.x) + (m_defocusDiskV * p.y);
}
