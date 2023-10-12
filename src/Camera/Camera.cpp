#include "../Math/Vector.h"
#include <cstdint>
#include "../Colour/Colour.h"
#include "../Objects/Hittable.h"
#include "../PPM/PPM.h"
#include "Camera.h"
#include "../Utility/Utility.h"
#include <iostream>

#include "../Materials/Material.h"

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
	m_focalLength = (m_lookFrom - m_lookAt).magnitude();

	auto theta = degrees_to_radians(m_vfov);
	auto H = std::tan(theta / 2);
	m_viewportHeight = 2 * H * m_focalLength;
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

	m_viewportUpperLeft = m_cameraCenter - (w * m_focalLength) - (m_viewportU / 2) - (m_viewportV / 2);
	m_pixel00Loc = m_viewportUpperLeft + ((m_pixelDeltaU + m_pixelDeltaV) * 0.5);

	//extra stuff
	m_samplesPerPixel = data.samplesPerPixel;
	m_maxRayBounces = data.maxRayBounces;
}


void Camera::render(const std::unique_ptr<HittableList>& HittableList, PPM& ppmOut)
{
	for (auto j = 0; j < ppmOut.getHeight(); ++j) {

		std::clog << "\rScanlines remaining: " << (ppmOut.getHeight() - j) << ' ' << std::flush;

		for (auto i = 0; i < ppmOut.getWidth(); ++i) {

			Colour PixelColour(0.0, 0.0, 0.0);

			for (int sample = 0; sample < m_samplesPerPixel; ++sample) {
				Ray r = getRay(i, j);
				PixelColour += this->getRayColour(r, m_maxRayBounces, HittableList);
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

	auto ray_origin = m_cameraCenter; //Doouble check this
	auto ray_direction = pixel_sample - ray_origin;

	return Ray(ray_origin, ray_direction);
}

Vector3 Camera::pixelSampleSquare() const
{
	auto px = -0.5 + genRandomDouble();
	auto py = -0.5 + genRandomDouble();
	return (m_pixelDeltaU * px) + (m_pixelDeltaV * py);
}
