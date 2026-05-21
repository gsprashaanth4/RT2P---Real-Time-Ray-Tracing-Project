/* This quat class is built to define, access and compute quaternions */

#ifndef QUAT_H
#define QUAT_H

#include <cmath>
#include <iostream>

#include "boiler.h"

class quat {
  public:
    double s;
    vec3 v;

    // constructors
    quat() : s(1.0), v(0.0, 0.0, 0.0) {}
    quat(double scalar, vec3 v) : s(scalar), v(v) {}
    quat(double scalar, double x, double y, double z) : s(scalar), v(x, y, z) {}

    // returning functions
    double w() const { return s; }
    double x() const { return v.x(); }
    double y() const { return v.y(); }
    double z() const { return v.z(); }

    quat operator~() const { return quat(s, -v); }   // conjugation operation

    // quaternion multiplication (operator overloading, post-multiplication)
    quat& operator*=(const quat& q) {
        *this = (*this) * q;
        *this = normalized();
        return *this;
    }

    // quaternion multiplication (post-multiplication)
    quat operator*(const quat& q) const {
        return quat(s * q.s - dot(v, q.v), s * q.v + q.s * v + cross(v, q.v));
    }

    quat normalized() const {
        double len = length();
        return quat(s/len, v/len);
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return s*s + v.length_squared();
    }

    static quat build(vec3 axis, double angle) {
        axis = unit_vector(axis);

        double deg_rad = angle*D2R;
        double half_rad = deg_rad * 0.5;

        return quat(cos(half_rad), axis * sin(half_rad));
    }

    vec3 rotate(vec3 p) const {
        quat pure(0, p);
        quat result = (*this) * pure * ~(*this);
        return result.v;
    }
};


#endif