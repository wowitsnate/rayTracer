#include <ostream>

#include "Vector.h"

#include <exception>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include "../Utility/Utility.h"

Vector3::Vector3()
{
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
}

Vector3::Vector3(float p_x, float p_y, float p_z)
{
	this->x = static_cast<double>(p_x);
	this->y = static_cast<double>(p_y);
	this->z = static_cast<double>(p_z);
}

Vector3::Vector3(int p_x, int p_y, int p_z)
{
	this->x = static_cast<double>(p_x);
	this->y = static_cast<double>(p_y);
	this->z = static_cast<double>(p_z);
}

Vector3::Vector3(double p_x, double p_y, double p_z)
{
	this->x = p_x;
	this->y = p_y;
	this->z = p_z;
}

Vector3 Vector3::VZero()
{
	return { 0.0, 0.0, 0.0 };
}

Vector3 Vector3::VUnit()
{
	return { 1.0, 1.0, 1.0 };
}

Vector3 Vector3::VInvalid()
{
	return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
}

bool Vector3::operator==(const Vector3& other) const
{
	//Should probably be a epsilon value so that fabs(this->x - other.x) < epsilon but idc
	return ((this->x == other.x) && (this->y == other.y) && (this->z == other.z));
}


bool Vector3::operator!=(const Vector3& other) const
{
	return !(*this == other);
}


Vector3 Vector3::operator+(const Vector3& other) const
{
	return { this->x + other.x, this->y + other.y, this->z + other.z };
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return { this->x - other.x, this->y - other.y, this->z - other.z };
}

Vector3 Vector3::operator-() const
{
	return { -this->x, -this->y, -this->z };
}

Vector3 Vector3::operator*(const double factor) const
{
	return { this->x * factor, this->y * factor, this->z * factor };
}

Vector3 Vector3::operator*(const Vector3& other) const
{
	return { x * other.x, y * other.y, z * other.z };
}

Vector3 Vector3::operator/(const double factor) const
{
	return { this->x / factor, this->y / factor, this->z / factor };
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

Vector3& Vector3::operator*=(const double factor) {
	x *= factor;
	y *= factor;
	z *= factor;
	return *this;
}

Vector3& Vector3::operator/=(const double factor) {
	if (factor != 0.0) {
		x /= factor;
		y /= factor;
		z /= factor;
	}
	// Handle division by zero (you can choose to throw an exception or handle it differently)
	else {
		// For example, throw an exception:
		throw std::invalid_argument("Division by zero in Vector3::operator/=");
	}
	return *this;
}

double Vector3::operator[](int i) const
{
	if ((i > 2) || (i < 0)) throw std::out_of_range("Index Out Of Range");


	//this may fuck up if any virtual functions are introduced but thats a problem for another day
	return reinterpret_cast<const double*>(this)[i];
}

double Vector3::dot(const Vector3& other) const
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3 Vector3::cross(const Vector3& other) const
{
	double crossX = (y * other.z) - (z * other.y);
	double crossY = (z * other.x) - (x * other.z);
	double crossZ = (x * other.y) - (y * other.x);

	return { crossX, crossY, crossZ };
}

double Vector3::magnitude() const
{
	return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalize() const
{
	double mag = this->magnitude();

	if (mag == 0.0)
	{
		//Goofy edge case
		return Vector3::VInvalid();
	}
	else
	{
		return *this / mag;
	}

}

double Vector3::squaredNorm() const
{
	return (x * x + y * y + z * z );
}

bool Vector3::isZero(double epsilon) const
{
	const bool b_x = fabs(this->x) < epsilon;
	const bool b_y = fabs(this->y) < epsilon;
	const bool b_z = fabs(this->z) < epsilon;

	return (b_x && b_y && b_z);
}

bool Vector3::isUnitVector(double epsilon) const
{
	const double mag = this->magnitude();
	return std::abs(mag - 1.0) < epsilon;
}

double Vector3::distance(const Vector3& other) const
{
	const Vector3 difference = *this - other;
	return difference.magnitude();
}


double Vector3::angleBetween(const Vector3& other) const
{
	// cos(theta) = (v1 * v2) / (|v1| * |v2|)

	const double dotProduct = this->dot(other);

	const double mag1 = this->magnitude();
	const double mag2 = other.magnitude();

	if (mag1 == 0.0 || mag2 == 0.0) throw std::invalid_argument("One of the vectors has zero magnitude.");

	double cosTheta = dotProduct / (mag1 * mag2);
	cosTheta = std::max(-1.0, std::min(1.0, cosTheta));
	return std::acos(cosTheta);
}

bool Vector3::isCollinear(const Vector3& other,const double epsilon) const
{
	const Vector3 crossProduct = this->cross(other);
	return crossProduct.magnitude() < epsilon;
}

void Vector3::normalizeInPlace()
{
	const double mag = magnitude();
	if (mag != 0.0)
	{
		this->x /= mag;
		this->y /= mag;
		this->z /= mag;
	}
}

Vector3 Vector3::reflect(const Vector3& other) const {
	return *this - (other * 2 * this->dot(other));
}

int Vector3::size()
{
	return 3;
}

Vector3 Vector3::lerp(const Vector3& start, const Vector3& end, double percent) {
	percent = std::max(0.0, std::min(1.0, percent)); // Ensure t is clamped between 0 and 1
	return (start * percent) + (end * (1.0 - percent));
}

Vector3 Vector3::slerp(const Vector3& start, const Vector3& end, const double percent) {
	double dot_prod = start.dot(end);

	dot_prod = std::clamp(dot_prod, -1.0, 1.0);

	const double theta = std::acos(dot_prod) * percent;

	Vector3 relative = (end - start) * dot_prod;
	relative.normalizeInPlace();

	return { (start * std::cos(theta)) + (relative * std::sin(theta)) };
}

Vector3 Vector3::random()
{
	return {
		genRandomDouble(),
		genRandomDouble(),
		genRandomDouble()
	};
}

Vector3 Vector3::random(double min, double max)
{
	return {
		genRandomDoubleRange(min, max),
		genRandomDoubleRange(min, max),
		genRandomDoubleRange(min, max)
	};
}
