#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
  public:
    // constructors
    ray() {}
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    const point3& origin() const   {return orig; }
    const vec3& direction() const  {return dir; }

    // where is it 'at' ? returning a point3
    point3 at(double t) const {
        return orig + t*dir;
    }

  private:
    point3 orig;
    vec3 dir;
};

#endif