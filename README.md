# Raytracer

Ray Tracer made following the Peter Shirley: Ray Tracing in one weekend book series [[1]](#1).

## Build

To build this repository you need to have installed CMake and clang++, For a debug build execute the command lines:  

```cmd
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -S . -B build -G "Unix Makefiles"
cmake --build build --config Debug --target all -j 6 --
```

This line will produce an executable at the relative path "./.package/debug/bin/Raytracer"

For a release build execute the command lines:  

```cmd
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -S . -B build -G "Unix Makefiles"
cmake --build build --config Release --target all -j 6 --
```

This line will produce an executable at the relative path "./.package/release/bin/Raytracer"

If needed use `-DCMAKE_C_COMPILER:FILEPATH=[Path to clang] -DCMAKE_CXX_COMPILER:FILEPATH=[Path to clang++]` in the first command of the debug and release preparation to add the compilers path to cmake build project

Executing the program will generate an image named "result.png" in the same path of the executable call.

## Project

The following section are my personal documented process of the project made following the books series.

### First book [[2]](#2)
<!-- markdownlint-disable html -->
<table cellspacing="0" cellpadding="0">
  <tr>
    <td rowspan="2">
        <img src="./renders/Ray Tracing in One Weekend/0_image.png"/>
        <figcaption>Fig.1 - Only a linear interpolation in the image coordinate.</figcaption>
    </td>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/1_blue_white.png"/>
        <figcaption>Fig.2 - Linear interpolation to simulate a sky.</figcaption>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/3_circle_with_normal.png"/>
        <figcaption>Fig.4 - Drawing the sphere normals vectors.</figcaption>
    </td>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/5_recursive_rays.png"/>
        <figcaption>Fig.6 Rendering with recursive rays without check for near collision.</figcaption>
    </td>
  </tr>
  <tr>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/2_circle.png"/>
        <figcaption>Fig.3 - Drawing the sphere only with color red on collision.</figcaption>
    </td>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/4_circle_with_floor.png" height=50%/>
        <figcaption>Fig.5 - Added another sphere as floor.</figcaption>
    </td>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/6_without_near_collision_gamma_corrected.png"/>
        <figcaption>Fig.7 - Gamma corrected and removed near collisions.</figcaption>
    </td>
  </tr>
  <tr>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/7_diffuse_and_metal_materials.png"/>
        <figcaption>Fig.8 - Added lambertian and meteal materials</figcaption>
    </td>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/9_dialectric_always_refract.png"/>
        <figcaption>Fig.10 - Added dialectric material with only refraction.</figcaption>
    </td>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/10_dialectric_with_reflection.png"/>
        <figcaption>Fig.11 - Added reflection in the dialectric material.</figcaption>
    </td>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/12_camera_with_vfov.png"/>
        <figcaption>Fig.13 - Test for the parametric vertical field of view.</figcaption>
    </td>
  </tr>
  <tr>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/8_metal_with_fuzz.png"/>
        <figcaption>Fig.9 - Added fuzz to the metal material.</figcaption>
    </td>
    <td colspan="2">
        <img src="./renders/Ray Tracing in One Weekend/11_ Schlick_reflectance.png"/>
        <figcaption>Fig.12 - Used Schlick approssimation for reflectance.</figcaption>
    </td>
    <td>
        <img src="./renders/Ray Tracing in One Weekend/13_movable_camera.png"/>
        <figcaption>Fig.14 - Moved the camera in another position.</figcaption>
    </td>
  </tr>
  <tr>
    <td colspan="2">
        <img src="./renders/Ray Tracing in One Weekend/14_less_vfov.png"/>
        <figcaption>Fig.15 - Reduced the field of view in the camera.</figcaption>
    </td>
    <td colspan="2">
        <img src="./renders/Ray Tracing in One Weekend/15_depth_of_field.png"/>
        <figcaption>Fig.16 - Added depth of field in the camera.</figcaption>
    </td>
  </tr>
  <tr>
    <td colspan="4">
        <img src="./renders/Ray Tracing in One Weekend/16_Final_Render.png"/>
        <figcaption>Fig.17 - Render of a random test scene with all the feature created in this raytracer.</figcaption>
    </td>
  </tr>
</table>
<!-- markdownlint-enable html -->

## Possible to do

- [ ] Lights — You can do this explicitly, by sending shadow rays to lights, or it can be done implicitly by making some objects emit light, biasing scattered rays toward them, and then downweighting those rays to cancel out the bias. Both work. I am in the minority in favoring the latter approach.
- [ ] Triangles — Most cool models are in triangle form. The model I/O is the worst and almost everybody tries to get somebody else’s code to do this.
- [ ] Surface Textures — This lets you paste images on like wall paper. Pretty easy and a good thing to do.
- [ ] Solid textures — Ken Perlin has his code online. Andrew Kensler has some very cool info at his blog.
- [ ] Volumes and Media — Cool stuff and will challenge your software architecture. I favor making volumes have the hittable interface and probabilistically have intersections based on density. Your rendering code doesn’t even have to know it has volumes with that method.
- [ ] Parallelism — Run 𝑁 copies of your code on 𝑁 cores with different random seeds. Average the 𝑁 runs. This averaging can also be done hierarchically where 𝑁/2 pairs can be averaged to get 𝑁/4 images, and pairs of those can be averaged. That method of parallelism should extend well into the thousands of cores with very little coding.

## Cites

<!-- markdownlint-disable html -->
- <a id="1">[1]</a>: [_Ray Tracing in One Weekend book series_](https://raytracing.github.io/)
- <a id="2">[2]</a>: [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- <a id="3">[3]</a>: [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- <a id="4">[4]</a>: [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
<!-- markdownlint-enable html -->
