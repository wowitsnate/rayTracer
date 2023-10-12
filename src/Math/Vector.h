#pragma once

#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_DEFAULT_EPSILON 1e-8



class Vector3
{
public:
	double x;
	double y;
	double z;

public:
	Vector3();
	Vector3(float p_x, float p_y, float p_z);
	Vector3(int p_x, int p_y, int p_z);
	Vector3(double p_x, double p_y, double p_z);

public:
	static Vector3 VZero();
	static Vector3 VUnit();
	static Vector3 VInvalid();

public:
	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;

	Vector3 operator+(const Vector3& other) const;
	Vector3 operator-(const Vector3& other) const;
	Vector3 operator-() const;
	Vector3 operator*(const double factor) const;
	Vector3 operator*(const Vector3& other) const;
	Vector3 operator/(const double factor) const;

	Vector3& operator+=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);
	Vector3& operator*=(const double factor);
	Vector3& operator/=(const double factor);

	double operator[](int i) const;

public:
	/*
	 * Iterator for range based for loops iterating through the vector. Iterates in this order: X, Y, Z
	 */
	class Iterator
	{
	private:
		double* ptr;

	public:
		Iterator(double* p) : ptr(p) {}

		double& operator*() const
		{
			return *ptr;
		}

		Iterator& operator++()
		{
			++ptr;
			return *this;
		}

		bool operator!=(const Iterator& other) const
		{
			return ptr != other.ptr;
		}
	};

	Iterator begin()
	{
		return { &x };
	}

	Iterator end()
	{
		return { &z + 1 };
	}

public:
	double dot(const Vector3& other) const;

	Vector3 cross(const Vector3& other) const;

	double magnitude() const;

	double squaredNorm() const;

	Vector3 normalize() const;

	bool isZero(double epsilon = VECTOR_DEFAULT_EPSILON) const;

	bool isUnitVector(double epsilon = VECTOR_DEFAULT_EPSILON) const;

	double distance(const Vector3& other) const;

	double angleBetween(const Vector3& other) const;

	bool isCollinear(const Vector3& other, double epsilon = VECTOR_DEFAULT_EPSILON) const;

	void normalizeInPlace();

	Vector3 reflect(const Vector3& other) const;

public:
	//This returns 3 for a vector 3. Important to note it is NOT the size(length) of the elements. That would be the magnitude
	static int size();

	static Vector3 lerp(const Vector3& start, const Vector3& end, double percent);

	static Vector3 slerp(const Vector3& start, const Vector3& end, double percent);

	static Vector3 random(); //Within the range 0.0->1.0

	static Vector3 random(double min, double max);
};

/*
inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
	return out << v.x << ' ' << v.y << ' ' << v.z;
}*/

inline Vector3 randomInUnitSphere()
{
	while (true) {
		auto p = Vector3::random(-1, 1);
		if (p.squaredNorm() < 1)
			return p;
	}
}

inline Vector3 randomUnitVector()
{
	return randomInUnitSphere().normalize();
}

inline Vector3 randomOnHemisphere(const Vector3& normal) {
	const Vector3 onUnitSphere = randomUnitVector();
	if (onUnitSphere.dot(normal) > 0.0)
		return onUnitSphere;
	else
		return -onUnitSphere;
}

using Point3 = Vector3;
using point3 = Vector3;

using vector3 = Vector3;
using Vec3 = Vector3;
using vec3 = Vector3;
#endif

