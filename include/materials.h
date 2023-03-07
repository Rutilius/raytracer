#pragma once

#include "rtweekend.h"
#include "material.h"
#include "hittable.h"
#include <cmath>


class lambertian : public material {
    public:
        lambertian(const color &a) : albedo(a) { }
        virtual ~lambertian() = default;

        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wunused-parameter"

        virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
            auto scatter_direction = random_in_hemisphere(rec.normal);

            if(scatter_direction.is_near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

        #pragma clang diagnostic pop

    public:
        color albedo;
};

class metal : public material {
    public: 
        metal(const color &a, const double f) : albedo(a), fuzz(f < 1.0 ? f : 1.0) { }
        virtual ~metal() = default;

        virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
            auto reflected = reflect(r_in.direction, rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return dot(scattered.direction, rec.normal) > 0.0;
        }
        
    public:
        color albedo;
        double fuzz;
};

class dielectric : public material {
    public: 
        dielectric(const double index_of_refraction) : ir(index_of_refraction) { }
        virtual ~dielectric() = default;

        virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
            attenuation = color(1.0);
            double refraction_ratio = rec.is_front_face ? (1.0 / ir) : ir;

            const double cos_theta = fmin(dot(r_in.direction, rec.normal), 1.0);
            const double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            const bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if(cannot_refract) {
                direction = reflect(r_in.direction, rec.normal);
            } else {
                direction = refract(r_in.direction, rec.normal, refraction_ratio);
            }

            scattered = ray(rec.p, direction);
            return true;
        }
        
    public:
        double ir; // Index of Refraction
};