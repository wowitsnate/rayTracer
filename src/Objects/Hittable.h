#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include <vector>
#include <memory>

#include "../Math/Interval.h"
#include "../Ray/Ray.h"


struct HitRecord {
	point3 p;
	vec3 normal;
	double t;
	bool frontFace;

	void set_face_normal(const Ray& r, const vec3& outward_normal);
};


//General Parent class for hittable objects (e.g. sphere, cube etc...)
class Hittable
{
public:
	Hittable() = default;
	~Hittable() = default;
	virtual bool intersects(const Ray& r, Interval ray_t, HitRecord& rec) { return false; }
};

class Sphere : public Hittable
{
private:
	Vector3 m_center;
	double m_radius;

public:
	Sphere(const Vector3& center, const double radius) : m_center(center), m_radius(radius) {}

	virtual bool intersects(const Ray& r, Interval ray_t, HitRecord& rec) override;
};

class HittableList : public Hittable
{
public:
	std::vector<std::shared_ptr<Hittable>> m_list = {};

public:
	HittableList() = default;

	void addObject(std::shared_ptr<Hittable> obj);

	virtual bool intersects(const Ray& r, Interval ray_t, HitRecord& rec) override;

	void clear();
};

#endif
