#include "boiler.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

int main () {
    // creating a hittable list 'world', which is also a hittable object throgh inheritance
    hittable_list world;

    // creating and adding hittable objects to the world
    world.add(make_shared<sphere>(point3(0,0,1), 0.5));
    world.add(make_shared<sphere>(point3(0,1,1), 0.5));
    world.add(make_shared<sphere>(point3(2,1,1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,1), 100));

    // creating a camera object
    camera cam1;

    // settting the public variables of the cam1 object
    cam1.aspect_ratio = 4.0 / 3.0;
    cam1.image_width  = 400;

    // rendering with the cam1 object
    cam1.render(world);
}
