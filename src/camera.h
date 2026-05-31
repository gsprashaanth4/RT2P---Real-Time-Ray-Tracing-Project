#ifndef CAMERA_H
#define CAMERA_H

#include "boiler.h"
#include "hittable.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class camera {
    public:
        /* Public Camera Parameters Here */
        double aspect_ratio = 4.0 / 3.0;        // defining the aspect ratio
        int image_width = 400;                  // defining the image width

        vec3 right_global   = vec3(1,0,0);      // defining global right vector
        vec3 up_global      = vec3(0,1,0);      // defining global up vector
        vec3 forward_global = vec3(0,0,1);      // defining global forward vector

        double movement_speed = 10;
        double rotation_speed = 20;
        
        void render(const hittable& world) {    // main rendering function, with the actually hittable_list world, but hittable through inheritance
            initialize();                       // initializing the camera and SDL assets

            // SDL running variables
            bool running = true;
            SDL_Event event;

            float fps = 0.0f;
            Uint32 lastFrameTime = SDL_GetTicks();
            SDL_Delay(1);

            while (running)
            {
                Uint32 frameBegin = SDL_GetTicks();     // getting the frame beginning time in ms
                double deltaTime = (frameBegin - lastFrameTime) / 1000.0;
                lastFrameTime = frameBegin;
                fps = 1.0/deltaTime;

                // clearing render screen
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);     // black color for initial screen clearing
                SDL_RenderClear(renderer);                          // clearing function from SDL

                while (SDL_PollEvent(&event)) {                     // loop for event polling
                    if (event.type == SDL_QUIT) {                   // handling window quit event
                        running = false;                            // changing running flag variable
                    }
                }

                const Uint8* keystate = SDL_GetKeyboardState(NULL); // gettting hte key states with SDL

                // yaw anti-clockwise -> 0 and increases            // conventions
                // pitch nose-down -> 360 and reduces               // conventions
                // roll left-wing down -> 360 and reduces           // conventions

                double app_yaw = 0.0;       // initiating the yaw angle applied for this iteration
                double app_pitch = 0.0;     // initiating the pitch angle applied for this iteration
                double app_roll = 0.0;      // initiating the roll angle applied for this iteration

                /* GETTING THE APPROPRIATE INPUTS FOR THE QUATERNION ROTATION OF THE CAMERA WITH THE CAPTURED KEYBOARD STATE */

                double time_scale_rot = rotation_speed * deltaTime;
                if (keystate[SDL_SCANCODE_J])                                   app_yaw   -= time_scale_rot;
                if (keystate[SDL_SCANCODE_L])                                   app_yaw   += time_scale_rot;
                if (keystate[SDL_SCANCODE_U])                                   app_roll  -= time_scale_rot;
                if (keystate[SDL_SCANCODE_O])                                   app_roll  += time_scale_rot;
                if (keystate[SDL_SCANCODE_I] && !keystate[SDL_SCANCODE_LSHIFT]) app_pitch -= time_scale_rot;
                if (keystate[SDL_SCANCODE_K] && !keystate[SDL_SCANCODE_LSHIFT]) app_pitch += time_scale_rot;

                // RESETTING CAMERA TRANSFORMATION USING 0 KEY
                if (keystate[SDL_SCANCODE_0]) {
                    camera_center = point3(0,0,0);
                    orientation   = quat(1.0, vec3(0.0, 0.0, 0.0));
                }

                // building the rotation quaternions with the recorded inputs
                quat yaw_rot   = quat::build(up_global,      app_yaw);
                quat pitch_rot = quat::build(right_global,   app_pitch);
                quat roll_rot  = quat::build(forward_global, app_roll);

                // APPLYING THE ROTAION QUATERNIONS TO THE ORIENTATION QUATERNION (INTRINSIC/lOCAL AXES ROTATION)
                // YAW FIRST, PITCH SECOND, ROLL THIRD APPLIED
                orientation *= yaw_rot;
                orientation *= pitch_rot;
                orientation *= roll_rot;

                // calculating the basis vectors of the camera from the current orientation quaternion
                vec3 forward = orientation.rotate(forward_global);
                vec3 up      = orientation.rotate(up_global);
                vec3 right   = orientation.rotate(right_global);

                double time_scale_pos = movement_speed * deltaTime;
                /* NOW USING THE KEYSTATES ONCE AGAIN TO APPLY TRANSLATIONS WITH RESPECT TO THE NEWLY CONSTRUCTED BASIS VECTORS */
                if (keystate[SDL_SCANCODE_W])                                   camera_center = camera_center + forward * time_scale_pos;
                if (keystate[SDL_SCANCODE_S])                                   camera_center = camera_center - forward * time_scale_pos;
                if (keystate[SDL_SCANCODE_D])                                   camera_center = camera_center + right   * time_scale_pos;
                if (keystate[SDL_SCANCODE_A])                                   camera_center = camera_center - right   * time_scale_pos;
                if (keystate[SDL_SCANCODE_I] && keystate[SDL_SCANCODE_LSHIFT])  camera_center = camera_center + up      * time_scale_pos;
                if (keystate[SDL_SCANCODE_K] && keystate[SDL_SCANCODE_LSHIFT])  camera_center = camera_center - up      * time_scale_pos;

                // calculating the viewport vectors from the new basis vectors of the camera
                vec3 viewport_u = right * viewport_width;
                vec3 viewport_v = -up * viewport_height;

                // calculating the delta step vectors from the computed viewport vectors
                vec3 pixel_delta_u = viewport_u / image_width;
                vec3 pixel_delta_v = viewport_v / image_height;

                // location of upper left pixel
                auto viewport_upper_left = camera_center                  // camera center vector
                                         + forward * focal_length         // - focal length (viewport infront of camera in negative z axis) = center of viewport in 3d space
                                         - viewport_u/2                   // - 1/2 of viewport width = left edge center of viewport
                                         - viewport_v/2;                  // - 1/2 of -viewport height = top-left corner of the viewport (positive Y axis up in 3d world)

                // starting (0,0) ray intersection point on the viewport
                auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

                // rendering loop
                for (int j = 0; j < image_height; j++) {                        // outer loop through image height
                    for (int i = 0; i < image_width; i++) {                     // inner loop through image width       
                    
                        // vector data type, calculating the pixel center point in the viewport
                        auto pixel_center = pixel00_loc                         // calculating pixel center for current center
                                          + (i * pixel_delta_u)                 // step-delta in width
                                          + (j * pixel_delta_v);                // step-delta in height        
                    
                        auto ray_direction = pixel_center - camera_center;      // calculating the ray direction of the ray cast from eye to the current pixel center
                        ray r(camera_center, ray_direction);                    // storing the ray direction as a ray itself
                        color pixel_color = ray_color(r, world);                // calculating the ray color for the point it intersects in the world of hittables

                        // calculating the color and storing it in memory, in variable pixels[]
                        pixels[j * image_width + i] = (int(255*pixel_color.x())) << 24 | 
                                                      (int(255*pixel_color.y())) << 16 | 
                                                      (int(255*pixel_color.z())) << 8  | 
                                                      255;
                    }
                }

                // updating the SDL texture screenTexture with the values stored in memory (pixels[] variable)
                SDL_UpdateTexture(screenTexture, NULL, pixels, image_width * sizeof(Uint32));
                SDL_RenderCopy(renderer, screenTexture, NULL, NULL);    // copying the texture onto the SDL renderer

                SDL_RenderPresent(renderer);                            // rendering the current screen
 
                // LOGGING in the CONSOLE
                std::cout << "X: " << camera_center.x() << "   Y: " << camera_center.y() << "   Z: " << camera_center.z() << "   FPS: " << fps << "\n";
            }
            SDL_DestroyWindow(window);  // destroying the SDL window
            SDL_Quit();                 // quitting SDL
        }

    private:

        /* Private Camera Variables Here */
        int image_height;           // variable to store the image height calculated from the image width and aspect ratio

        double focal_length;        // focal length of the camera (distance between the camera and the viewport)
        double viewport_height;     // the viewport's height stored
        double viewport_width;      // the viewport's width stored

        point3 camera_center;       // the camera center point3 object

        Uint32* pixels;             // the variable to store the pixel colors in memory

        quat orientation;           // initiating the orientation quaternion

        SDL_Window *window;         // the SDL window object initiation
        SDL_Renderer *renderer;     // the SDL renderer object initiation
        SDL_Texture* screenTexture; // the SDL asset to store the texture/image to be rendered
        
        void initialize() {

            // CAMERA CONFIG ////////////////////////////////////////////////////////////////////////////////////////
            
            image_height = int(image_width / aspect_ratio);         // calculating image height

            // camera and viewport setup
            focal_length = 1.0;                                     // focal length - camera to viewport distance
            viewport_height = 1.0;                                  // height of viewport (arbitrary)
            viewport_width = viewport_height 
                            * (double(image_width)/image_height);   // width of viewport based on obtained aspect ratio
            
            camera_center = point3(0,0,0);                          // defining center of the camera - eye
            orientation = quat(1.0, vec3(0.0, 0.0, 0.0));           // defining the orientation quaternion of the camera
            pixels = new Uint32[image_width * image_height];        // initiating the memory object pixels[] to store the pixel colors

            // SDL CONFIG ///////////////////////////////////////////////////////////////////////////////////////////

            // init of SDL check
            if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                std::cout << "SDL could not be initiated: " << SDL_GetError();
            }
            else {
                std::cout << "SDL video system is ready to go\n";
            }

            // creating SDL window 
            window = SDL_CreateWindow(
                "RT^2-P",               // Title
                SDL_WINDOWPOS_CENTERED, // X position
                SDL_WINDOWPOS_CENTERED, // Y position
                image_width*2,          // Width
                image_height*2,         // Height
                SDL_WINDOW_SHOWN        // Flags
            );

            // SDL window created or not check
            if (window == NULL) {
                std::cout << "Window could not be created! Error: " << SDL_GetError() << std::endl;
                SDL_Quit();
            }

            // SDL renderer creation
            renderer = SDL_CreateRenderer(
                window,
                -1,
                SDL_RENDERER_ACCELERATED
            );

            // renderer check
            if (renderer == NULL) {
                std::cout << "Renderer could not be created! Error: " << SDL_GetError() << std::endl;
                SDL_Quit();
            }

            // initiating hte screen texture object
            screenTexture = SDL_CreateTexture(
                            renderer,
                            SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_STREAMING,
                            image_width,
                            image_height
                        );
        }
        
        // FUNCTION TO RETURN THE PIXEL COLOR, GIVEN RAY AND THE WORLD HITTABLE OBJECT
        color ray_color(const ray& r, const hittable& world) const {
            
            /* ANYTHING HIT IN THE WORLD */
            hit_record rec;
            if(world.hit(r, interval(0, infinity), rec)) {  // rec stores any data you may deem necessary, regarding the current ray
                return 0.5 * (rec.normal + color(1,1,1));   // rec here used to retrieve the normal of the hit surface
            }

            /* NO HIT */
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        }
};

#endif
