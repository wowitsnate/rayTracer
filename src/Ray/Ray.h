#pragma once

#ifndef RAY_H
#define RAY_H

#include <memory>
#include "../Colour/Colour.h"


class HittableList;
class Hittable;

class Ray
{
public:
	Point3 origin;
	Vector3 direction;

public:
	Ray(const Point3& p_origin, const Vector3& p_direction) : origin(p_origin), direction(p_direction) {}

	Point3 at(const double t) const;
};



#endif