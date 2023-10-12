#pragma once

#ifndef UTILITY_H
#define UTILITY_H

#include <random>

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

#endif