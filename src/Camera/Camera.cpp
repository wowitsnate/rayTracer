#include "../Math/Vector.h"
#include <cstdint>
#include "../Colour/Colour.h"
#include "../Objects/Hittable.h"
#include "../PPM/PPM.h"
#include "Camera.h"
#include <iostream>

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
}

void Camera::render(const std::unique_ptr<HittableList>& HittableList, PPM& ppmOut)
{
	for (auto j = 0; j < ppmOut.getHeight(); ++j) {

		std::clog << "\rScanlines remaining: " << (ppmOut.getHeight() - j) << ' ' << std::flush;

		for (auto i = 0; i < ppmOut.getWidth(); ++i) {

			auto pixelCenter = this->getPixelCenter(i, j);
			auto ray_direction = pixelCenter - this->getCameraCenter();

			Ray r(this->getCameraCenter(), ray_direction);

			Colour<int> colData = this->getRayColour(r, HittableList);
			ppmOut.m_data.push_back(colData);
		}
	}
}

Vector3 Camera::getPixelCenter(const int i, const int j) const
{
	return m_pixel00Loc + (m_pixelDeltaU * i) + (m_pixelDeltaV * j);
}

Colour<int> Camera::getRayColour(const Ray& r, const std::unique_ptr<HittableList>& HittableList)
{
	HitRecord rec;
	if (HittableList->intersects(r, Interval(0.0, std::numeric_limits<double>::infinity()), rec)) {
		Vector3 colTemp = (rec.normal + Vec3{1.0, 1.0, 1.0}) * 0.5;
		return {
			static_cast<int>(colTemp.x * 255),
			static_cast<int>(colTemp.y * 255),
			static_cast<int>(colTemp.z * 255)
		};
	}

	const Vector3 unitDir = r.direction.normalize();
	const auto a = 0.5 * (unitDir.y + 1.0);
	const auto res = Vector3::lerp({ 0.5, 0.7, 1.0 }, { 1.0, 1.0, 1.0 }, a);

	return { static_cast<int>(res.x * 255.0), static_cast<int>(res.y * 255), static_cast<int>(res.z * 255) };
}
