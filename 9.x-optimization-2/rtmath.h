#ifndef RTMATH_H
#define RTMATH_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    thread_local static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max)
{
    static std::uniform_real_distribution<double> distribution(min, max);
    thread_local  static std::mt19937 generator;
    return distribution(generator);
}

// inline double randome_double(double min, double max) {
//     return min + (max-min) * random_double();
// }

inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

#endif