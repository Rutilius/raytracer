#pragma once

#include <cmath>
#include <random>
#include <cstdlib>
#include <limits>
#include <memory>

// Common Headers
#include "ray.h"
#include "vec3.h"

// Constants
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;
constexpr double tao = 2.0 * pi;

// Utility Functions
constexpr double degree_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    thread_local std::random_device rd;
    thread_local std::mt19937 rng(rd());  
    thread_local std::uniform_int_distribution<int> uid;
    return uid(rng, decltype(uid)::param_type{0, RAND_MAX}) / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max - min)*random_double();
}

inline double clamp(double x, double min, double max) {
    if(x < min) { return min; }
    if(x > max) { return max; }
    return x; 
}

inline vec3 random_vec3() {
    return vec3(random_double(), random_double(), random_double());
}

inline static vec3 random_vec3(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline vec3 random_in_unit_sphere() {
    /*
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1));
        if (p.length_squared() >= 1) continue;
        return normalized(p);
    }
    */

    return vec3(cos(random_double(0.0, tao)), cos(random_double(0.0, tao)), cos(random_double(0.0, tao)));
}

inline vec3 random_in_unit_disk_full() {
    /*
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
    */

    return vec3(cos(random_double(0.0, tao)), sin(random_double(0.0, tao)), 0.0) * random_double();
}

inline vec3 random_in_hemisphere(const vec3 &normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    return dot(in_unit_sphere, normal) > 0.0 ? in_unit_sphere : -in_unit_sphere;
}