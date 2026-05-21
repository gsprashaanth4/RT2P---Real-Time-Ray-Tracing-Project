#ifndef BOILER_H
#define BOILER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// C++ Std Usings
using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double D2R = 0.0174533; // 1° = 0.0174533 rad

// Common Headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include "quat.h"

#endif