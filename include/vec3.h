#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <format>

using std::sqrt;

struct vec3 {
    public:
        constexpr vec3() { x = 0; y = 0; z = 0; }
        constexpr vec3(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }

        constexpr vec3 operator - () const { return vec3(-x, -y, -z); }

        double zero = 0;
        constexpr double operator [] (int i) const { 
            assert(i > 0 && i < 3 );
            switch (i) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                default: throw std::out_of_range("Index required is: " + std::to_string(i) + ", the valid range is: [0, 2]") ;
            }
        }
        constexpr double& operator [] (int i) {      
            assert(i > 0 && i < 3 );
            switch (i) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                default: throw std::out_of_range("Index required is: " + std::to_string(i) + ", the valid range is: [0, 2]") ;
            }
        }

        constexpr vec3& operator += (const vec3 &v) {
            x += v.x;
            y += v.y;
            y += v.z;
            return *this;
        }

        constexpr vec3& operator *= (const double t) {
            x *= t;
            y *= t;
            z *= t;
            return *this;
        }

        constexpr vec3& operator /= (const double t) {
            return *this *= 1.0 / t;
        }

        constexpr double length() const {
            return sqrt(length_squared());
        }

        constexpr double length_squared() const {
            return x*x + y*y + z*z;
        }
    
    public:
        double x;
        double y;
        double z; 
};

using point3 = vec3;
using color = vec3;

// vec3 Utilities Functions

inline constexpr std::ostream& operator << (std::ostream &out, const vec3 &v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline constexpr vec3 operator + (const vec3 &v1, const vec3 &v2) {
    return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline constexpr vec3 operator - (const vec3 &v1, const vec3 &v2) {
    return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline constexpr vec3 operator * (const vec3 &v1, const vec3 &v2) {
    return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline constexpr vec3 operator * (const double t, const vec3 &v) {
    return vec3(t * v.x, t * v.y, t * v.z);
}

inline constexpr vec3 operator * (const vec3 &v, const double t) {
    return t * v;
}

inline constexpr vec3 operator / (const vec3 &v, const double t) {
    return (1 / t) * v;
}

inline constexpr double dot(const vec3 &v1, const vec3 &v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

inline constexpr vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.y*v2.z - v1.z*v2.y, 
                v1.z*v2.x - v1.x*v2.z,
                v1.x*v2.y - v1.y*v2.x);
}

inline constexpr vec3 normalized(const vec3 &v) {
    return v / v.length();
}