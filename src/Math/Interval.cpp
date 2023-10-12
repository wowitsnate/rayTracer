
#include "Interval.h"

bool Interval::contains(const double x) const
{
	return min <= x && x <= max;
}

bool Interval::surrounds(const double x) const
{
	return min < x && x < max;
}

double Interval::clamp(const double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
