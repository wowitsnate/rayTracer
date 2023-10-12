#ifndef INTERVAL_H
#define INTERVAL_H
#include <limits>

const double infinity = std::numeric_limits<double>::infinity();

class Interval
{
public:
	double min;
	double max;

public:
	Interval() : min(+infinity), max(-infinity) {}
	Interval(const double p_min, const double p_max) : min(p_min), max(p_max) {}

public:
	bool contains(double x) const;
	bool surrounds(double x) const;
};

const static Interval empty(+infinity, -infinity);
const static Interval universe(-infinity, +infinity);

#endif