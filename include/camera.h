#pragma once

#include "rtweekend.h"


class camera {
    public:
        camera(
                const point3 lookfrom, const point3 lookat, const vec3 vup,
                const double vfov, // vertical field-of-view in degrees
                const double aspect_ratio,
                const double aperture,
                const double depth_of_field
            ) {

            const auto theta = degree_to_radians(vfov);
            const auto h = tan(theta / 2.0); 
            const auto viewport_height = 2.0 * h;
            const auto viewport_width = viewport_height * aspect_ratio;

            w = normalized(lookfrom - lookat);
            u = normalized(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = depth_of_field * viewport_width * u;
            vertical = depth_of_field * viewport_height * v;
            lower_left_corner = origin - horizontal/2.0 - vertical/2.0 - depth_of_field*w;

            lens_radius = aperture / 2.0;
        }

        ray get_ray(double s, double t) const {
            const vec3 rd = lens_radius * random_in_unit_disk_full();
            const vec3 offset = u*rd.x + v*rd.y;

            return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }
    
    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
};