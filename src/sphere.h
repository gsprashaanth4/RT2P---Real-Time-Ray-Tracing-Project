#ifndef SPHERE_H
#define SPHERE_H

#include "boiler.h"
#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:

        // constructor
        sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

        // the hit function derived from the sphere equation
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = h*h - a*c;
            if (discriminant < 0)
                return false;

            auto sqrtd = std::sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root))
                    return false;
            }

            rec.t = root;                                       // storing the computed root
            rec.p = r.at(rec.t);                                // the point at which the ray has hit
            vec3 outward_normal = (rec.p - center) / radius;    // computing the outward normal
            rec.set_face_normal(r, outward_normal);             // setting the normal based on in-travelling or out-travelling ray

            return true;
        }

    private:
        point3 center;  // the center of the sphere
        double radius;  // the radius of the sphere
};

#endif