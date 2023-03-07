#pragma once

class material;

#include "hittable.h"
#include "rtweekend.h"

class material {
    public:
        virtual ~material() = default;
        virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

