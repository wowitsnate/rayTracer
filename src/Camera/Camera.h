#pragma once

#ifndef CAMERA_H
#define CAMERA_H
#include <memory>

struct CameraInitData
{
	uint32_t imageWidth; //Can maybe just be replaced with a PPM* ?
	uint32_t imageHeight;

	Point3 cameraCenter;

	double aspectRatio;
	double focalLength;
	double viewportHeight;

	int samplesPerPixel;
	int maxRayBounces;
};

class Camera
{
private:
	double m_aspectRatio;
	double m_focalLength;

	double m_viewportHeight;
	double m_viewportWidth;

	Point3 m_cameraCenter;

	Vector3 m_viewportU;
	Vector3 m_viewportV;

	Vector3 m_pixelDeltaU;
	Vector3 m_pixelDeltaV;

	Vector3 m_viewportUpperLeft;
	Vector3 m_pixel00Loc;

	int m_samplesPerPixel;
	int m_maxRayBounces;

public:
	Camera(const CameraInitData& data);

	void render(const std::unique_ptr<HittableList>& HittableList, PPM& ppmOut);

	Vector3 getPixelCenter(const int i, const int j) const;

	Point3 getCameraCenter() const { return m_cameraCenter; }

	Colour getRayColour(const Ray& r, int depth, const std::unique_ptr<HittableList>& HittableList);

private:
	Ray getRay(int i, int j);

	Vector3 pixelSampleSquare() const;

};

#endif