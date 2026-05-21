#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "boiler.h"
#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

// hittable is a derived class that 
class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    // constructor
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    // function to check if a ray r has hit anything in a hittable list
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;                    // record for that one ray r, encompassing all the objects in the hittable list
        bool hit_anything = false;              // recording if anything hit
        auto closest_so_far = ray_t.max;        // recording the closest hit point for the current ray r

        // iterating through every object in the hittable list (an instance of the hittable list type actually)
        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) { // calling the current object's sub-class's hit funtion along with the temp-record
                hit_anything = true;            // making the bool true, as something is hit
                closest_so_far = temp_rec.t;    // changing the closest so far distance in the remp-record
                rec = temp_rec;                 // updating the main record sent in the hit function call by the hittable_list instance
            }
        }

        return hit_anything;
    }
};

#endif