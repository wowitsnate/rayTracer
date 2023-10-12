#pragma once

#ifndef COLOUR_H
#define COLOUR_H

#include <string>

//For Colours. Stores in RGB format but has member functions for converting to other colour spaces
template <typename t>
class Colour
{
public:
	t r;
	t g;
	t b;
public:
	Colour(t p_r, t p_g, t p_b);
	Colour() = default;

	Colour<t> operator*(const double factor) const;

public:
	std::string asString() const;
};

template<typename t>
Colour<t>::Colour(t p_r, t p_g, t p_b)
{
	r = p_r;
	g = p_g;
	b = p_b;
}

template<typename t>
inline Colour<t> Colour<t>::operator*(const double factor) const
{
	return Colour<t>(static_cast<t>(r * factor), static_cast<t>(g * factor), static_cast<t>(b * factor));
}

template<typename t>
std::string Colour<t>::asString() const
{
	std::string res = {};
	res.append(std::to_string(r) + " ");
	res.append(std::to_string(g) + " ");
	res.append(std::to_string(b));
	return res;
}

#endif

