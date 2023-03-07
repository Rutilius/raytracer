#pragma once

#include "rtweekend.h"


class camera {
    public:
        camera(
                const point3 lookfrom, const point3 lookat, const vec3 vup,
                const double vfov, // vertical field-of-view in degrees
                const double aspect_ratio
            ) {

            const auto theta = degree_to_radians(vfov);
            const auto h = tan(theta / 2.0); 
            const auto viewport_height = 2.0 * h;
            const auto viewport_width = viewport_height * aspect_ratio;

            const auto w = normalized(lookfrom - lookat);
            const auto u = normalized(cross(vup, w));
            const auto v = cross(w, u);

            origin = lookfrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal/2.0 - vertical/2.0 - w;
        }

        ray get_ray(double u, double v) const {
            return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }
    
    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};