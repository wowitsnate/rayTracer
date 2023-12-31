#pragma once

#ifndef COLOUR_H
#define COLOUR_H

#include <string>

//Stores colours as doubles.
// RGB ranging from 0->1
class Colour
{
public:
	double r;
	double g;
	double b;
public:
	Colour(double p_r, double p_g, double p_b);
	Colour() : r(0.0), g(0.0), b(0.0) {};

	Colour operator*(const double factor) const;
	Colour operator*(const Colour& other) const;
	void operator*=(const double factor);
	void operator+=(const Colour& other);

public:
	//Converts from 0->1 to 0 -> 255 for convenience
	std::string asString() const;

	static Colour randColour();
	static Colour randColour(double min, double max);
};




#endif

