#include "Colour.h"
#include <algorithm>

Colour::Colour(double p_r, double p_g, double p_b)
{
	r = p_r;
	g = p_g;
	b = p_b;
}

Colour Colour::operator*(const double factor) const
{
	return {
		this->r * factor,
		this->g * factor,
		this->b * factor
	};
}

Colour Colour::operator*(const Colour& other) const
{
	return { r * other.r, g * other.g, b * other.b };
}

void Colour::operator*=(const double factor)
{
	this->r = this->r * factor;
	this->g = this->g * factor;
	this->b = this->b * factor;
}

void Colour::operator+=(const Colour& other)
{
	this->r = this->r + other.r;
	this->g = this->g + other.g;
	this->b = this->b + other.b;
}




std::string Colour::asString() const
{
	std::string res = {};
	res.append(std::to_string(static_cast<int>(r * 255.0)) + " ");
	res.append(std::to_string(static_cast<int>(g * 255.0)) + " ");
	res.append(std::to_string(static_cast<int>(b * 255.0)));
	return res;
}



/*
inline Colour Colour::operator*(const double factor) const
{
	return {
		std::clamp<double>(this->r * factor, 0.0, 1.0),
		std::clamp<double>(this->g * factor, 0.0, 1.0),
		std::clamp<double>(this->b * factor, 0.0, 1.0)
	};
}
*/