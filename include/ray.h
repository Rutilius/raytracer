#pragma once

#import "vec3.h"

struct ray {
    public:
        ray() {}
        ray(const point3 &origin, const vec3 &direction) : origin(origin), direction(direction) {}

        constexpr point3 at(const double t) const {
            return origin + t*direction;
        }

    public: 
        point3 origin;
        vec3 direction;
};