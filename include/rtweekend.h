#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Common Headers
#include "ray.h"
#include "vec3.h"

// Constants
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

// Utility Functions
constexpr double degree_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max - min)*random_double();
}

inline double clamp(double x, double min, double max) {
    if(x < min) { return min; }
    if(x > max) { return max; }
    return x; 
}

inline vec3 random_in_unit_sphere() {
    double x = sin(random_double(0.0, pi));
    double y = sin(random_double(0.0, pi));
    double z = sin(random_double(0.0, pi));

    double m = random_double(0.0, 1.0);

    return vec3(x, y, z) * m;
}
