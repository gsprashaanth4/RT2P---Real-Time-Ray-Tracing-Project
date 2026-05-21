#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// a record class to record the ray hitting details for any hittable object
class hit_record {
  public:
    point3 p;           // point of hit
    vec3 normal;        // the normal calculated at that point
    double t;           // the length of the ray from the camera(eye) to the point
    bool front_face;    // noting if the light is from inside or outside the object (here sphere so far)

    // function to set the front_face boolean
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal; // inverting the normal, inwards of the sphere if the ray travels from inside it
    }
};

// the hittable class
class hittable {
  public:
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif