# RT²P — Real-Time Ray Tracing Project

A custom-built real-time ray tracing project written in C++, inspired by the foundational ideas presented in the *Ray Tracing in One Weekend* book series by Peter Shirley.

This project originally started as an implementation of the concepts covered in the book, but eventually diverged into a much larger experimental rendering system focused on:

- real-time ray traced rendering
- quaternion-based orientation systems
- fully unrestricted 6DOF camera movement
- interactive viewport rendering using SDL2
- low-level rendering architecture experimentation

The long-term goal of this project is the gradual development of a custom **real-time ray tracing engine** built from first principles.

---

# Current State of the Project

This repository represents an **early-stage foundation build** of the renderer.

The current implementation focuses heavily on:
- renderer architecture experimentation
- camera mathematics
- quaternion rotation systems
- interactive real-time rendering infrastructure

rather than advanced rendering features.

The codebase is intentionally being developed incrementally while documenting the process through slower-paced technical writeups and learning-focused explanations on platforms like:
- Medium
- Instructables

This repository should therefore be viewed as:
> an evolving rendering-engine development project

rather than a finished renderer.

---

# Starting Point

This project was originally based on the implementation style and rendering pipeline from:<br>
[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

More specifically, at the given moment the project follows the architecture and concepts roughly up to the:
> "Moving Camera Code Into Its Own Class"

portion of the book before diverging into custom systems.

From that point onward, several major systems were redesigned and extended independently.

---

# Major Custom Systems

## Quaternion Mathematics System (`quat.h`)

One of the most important custom-built components in this project is the quaternion implementation contained in:

```cpp
quat.h
```

This class was developed specifically to support unrestricted 3D rotational control for the renderer's camera system.

### Features Implemented

- Quaternion multiplication
- Quaternion normalization
- Quaternion conjugation
- Axis-angle quaternion construction
- Vector rotation using quaternion transforms
- Rotation composition through post-multiplication

### Example Rotation Pipeline

```cpp
quat yaw_rot   = quat::build(up_global, app_yaw);
quat pitch_rot = quat::build(right_global, app_pitch);
quat roll_rot  = quat::build(forward_global, app_roll);

orientation *= yaw_rot;
orientation *= pitch_rot;
orientation *= roll_rot;
```

### Why Quaternions?

The renderer originally relied on conventional rotational approaches, but the project eventually required:
- unrestricted camera movement
- local-axis rotation
- stable compound orientation updates

Using quaternions helped avoid common Euler-angle issues such as:
- gimbal lock
- axis-order instability
- accumulated rotational inconsistencies

The quaternion system now forms the mathematical foundation of the renderer's camera orientation logic.

---

# Fully Custom 6DOF Camera System

A major focus of this project is the development of a true **6 Degrees of Freedom (6DOF)** camera system.

Unlike conventional FPS-style cameras constrained by yaw/pitch movement, this implementation allows:
- free rotational movement
- intrinsic/local-axis rotations
- unrestricted translation in 3D space

The camera system dynamically reconstructs its basis vectors every frame using the current orientation quaternion.

### Dynamic Basis Reconstruction

```cpp
vec3 forward = orientation.rotate(vec3(0,0,-1));
vec3 up      = orientation.rotate(vec3(0,1,0));
vec3 right   = orientation.rotate(vec3(1,0,0));
```

These updated basis vectors are then used for:
- viewport reconstruction
- ray generation
- camera-space translation
- local-axis movement

---

# Real-Time Interactive Rendering

Unlike the offline rendering approach used in the original educational implementation, this project introduces a real-time interactive rendering loop.

The renderer:
- continuously redraws frames
- processes keyboard input
- updates orientation in real time
- reconstructs viewport geometry every frame
- streams rendered pixels directly into an SDL texture

---

# Interactive Controls

## Rotation Controls

| Key | Action |
|---|---|
| `Q` | Yaw left |
| `E` | Yaw right |
| `I` | Pitch up |
| `K` | Pitch down |
| `J` | Roll left |
| `L` | Roll right |

## Translation Controls

| Key | Action |
|---|---|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Move left |
| `D` | Move right |
| `Shift + I` | Move upward |
| `Shift + K` | Move downward |

## Reset

| Key | Action |
|---|---|
| `0` | Reset camera position and orientation |

---

# Rendering Architecture

The current renderer architecture includes:

| Component | Purpose |
|---|---|
| `camera.h` | Camera system and realtime rendering loop |
| `quat.h` | Quaternion mathematics implementation |
| `hittable.h` | Abstract hittable interface |
| `hittable_list.h` | Scene object container |
| `sphere.h` | Sphere primitive intersections |
| `ray.h` | Ray representation |
| `boiler.h` | Shared utility/math functionality |

---

# Current Rendering Features

## Implemented

- Sphere-ray intersections
- Surface normal visualization
- Real-time viewport rendering
- Dynamic camera orientation
- Quaternion-based camera rotations
- Local-axis camera translation
- SDL texture streaming
- Sky gradient rendering
- Interactive camera controls

## Planned Future Features

The long-term direction of the project includes experimentation with:

- materials
- reflections
- refractions
- acceleration structures (BVH)
- multithreading
- progressive accumulation
- denoising
- physically-based rendering
- better sampling systems
- GPU acceleration experiments
- renderer optimization
- engine-style architecture

---

# Technical Focus of the Project

This project is heavily focused on learning and experimentation in areas such as:

- computer graphics
- rendering mathematics
- low-level engine architecture
- quaternion mathematics
- camera transformation systems
- realtime rendering pipelines
- ray-object intersection systems
- rendering performance experimentation

---

# Build Example

## Dependencies

- C++17 compatible compiler
- SDL2
- CMAKE

## Build and Run

```bash
mkdir build && cd build
cmake ..
make
./main
```

---

# Important Notes

This is not intended to be:
- a production renderer
- a graphics framework
- a polished engine

The project exists primarily as:
- a learning platform
- a rendering research sandbox
- a mathematics and graphics experimentation environment

The implementation style intentionally prioritizes:
- clarity
- architectural experimentation
- low-level understanding

over abstraction-heavy engine design.

---

# Future Documentation

Topics planned for deeper breakdowns in aforementioned forums include:
- quaternion mathematics and implementation
- 6DOF camera mathematics
- ray generation
- viewport reconstruction
- realtime rendering architecture
- SDL framebuffer integration
- rendering optimization experiments

---

# Acknowledgements

Special thanks to: [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

for providing one of the best introductions to practical ray tracing and rendering fundamentals.

---

# License

This project is licensed under the MIT License.

---

# GIF

![Demo](https://github.com/gsprashaanth4/RT2P---Real-Time-Ray-Tracing-Project/blob/main/media/rt.gif)