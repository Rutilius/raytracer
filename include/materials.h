#pragma once

#import "material.h"
#include "rtweekend.h"
#import "hittable.h"

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