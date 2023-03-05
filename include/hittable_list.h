#pragma once

#include "hittable.h"
#include <cmath>
#include <memory>
#include <vector>

using std::shared_ptr;

class hittable_list : public hittable {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }
        virtual ~hittable_list() = default;

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    public:
        std::vector<shared_ptr<hittable>> objects;
};

inline bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto &object  : objects) {
        if(object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}