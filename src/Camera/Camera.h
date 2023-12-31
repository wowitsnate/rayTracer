#pragma once

#ifndef CAMERA_H
#define CAMERA_H
#include <memory>

/*
 Image width
 Aspect Ratio
 VFov
 samplesPerPixel
 maxRaybounces

 Lookfrom vec3
 lookat vec3
 */


struct CameraInitData
{
	int imageWidth;
	double aspectRatio;
	double vFov;

	int samplesPerPixel;
	int maxRayBounces;

	Vector3 lookFrom;
	Vector3 lookAt;
	Vector3 vUp;

	double defocusAng;
	double focusDist;
};

class Camera
{
public:
	double m_aspectRatio;
	int m_samplesPerPixel;
	int m_maxRayBounces;
	double m_vfov;

	double m_defocusAngle;
	double m_focusDist;

	uint32_t m_imageHeight;
	uint32_t m_imageWidth;

	Point3 m_lookFrom = point3(0, 0, -1);
	Point3 m_lookAt = point3(0, 0, 0);
	Vector3 m_vUp = vec3(0, 1, 0); 

private:
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

	Vector3 m_defocusDiskU;
	Vector3 m_defocusDiskV;

	Vec3 u, v, w; //Camera frame basis vectors;

	

public:
	Camera(const CameraInitData& data);

	void render(const std::unique_ptr<HittableList>& HittableList, PPM& ppmOut);

	Vector3 getPixelCenter(const int i, const int j) const;

	Point3 getCameraCenter() const { return m_cameraCenter; }

	Colour getRayColour(const Ray& r, int depth, const std::unique_ptr<HittableList>& HittableList);

	Ray getRay(int i, int j);

private:
	

	Vector3 pixelSampleSquare() const;

	Point3 defocusDiskSample() const;

	

};

#endif