#pragma once

#ifndef UTILITY_H
#define UTILITY_H

#include <random>


# define M_PI           3.14159265358979323846  /* pi */


static std::random_device rd;
static std::mt19937 gen(rd());


inline double genRandomDouble()
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(gen);
}

inline double genRandomDoubleRange(const double min, const double max)
{
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(gen);
}

inline double degrees_to_radians(double degrees) {
    return degrees * M_PI / 180.0;
}


#endif