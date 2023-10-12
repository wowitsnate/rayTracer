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
	m_aspectRatio = data.aspectRatio;
	m_focalLength = data.focalLength;

	m_viewportHeight = data.viewportHeight;
	m_viewportWidth = data.viewportHeight * (static_cast<double>(data.imageWidth) / data.imageHeight);

	m_cameraCenter = data.cameraCenter;

	m_viewportU = Vector3{ m_viewportWidth, 0.0, 0.0 };
	m_viewportV = Vector3{ 0.0, -m_viewportHeight, 0.0 };

	m_pixelDeltaU = m_viewportU / data.imageWidth;
	m_pixelDeltaV = m_viewportV / data.imageHeight;

	m_viewportUpperLeft = m_cameraCenter - vec3(0.0, 0.0, m_focalLength) - (m_viewportU / 2) - (m_viewportV / 2);
	m_pixel00Loc = m_viewportUpperLeft + ((m_pixelDeltaU + m_pixelDeltaV) * 0.5);

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
