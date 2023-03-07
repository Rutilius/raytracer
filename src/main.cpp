#include "color.h"
#include "hittable.h"
#include "rtweekend.h"

#include <iostream>
#include <memory>
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"
#include "camera.h"
#include "materials.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

using std::make_shared;


color ray_color(const ray &r, const hittable &world, int depth) {
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if(depth <= 0) { return color(0.0); }

    hit_record rec;
    if(world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if(rec.material->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color(0.0);
    }
    auto t = 0.5 * (r.direction.y + 1.0);
    return (1.0 - t)*color(1.0) + t*color(0.5, 0.7, 1.0);
}

int main(int, char**) {
    const char *result_path = "result.png";

    // Image
    constexpr int channels = 3;
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 600;
    constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
    constexpr int sample_per_pixel = 100;
    constexpr int max_depth = 50;

    // World
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>     (color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left  ));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left  ));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right ));

    // Camera
    camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 90, aspect_ratio);

    // Render
    stbi_flip_vertically_on_write(true);
    std::unique_ptr<char[]> img = std::make_unique<char[]>(image_width * image_height * channels);

    for(int j = image_height - 1; j >= 0 ; --j) {
        std::cout << "\rScanlines remaining: " << j << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0);
            
            for (int s = 0; s < sample_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            auto start_color_index((j * channels * image_width) + i*channels);
            write_color(img.get(), start_color_index, pixel_color, sample_per_pixel);
        }
    }

    stbi_write_png(result_path, image_width, image_height, channels, img.get(), image_width * channels);
    
    std::cout << "\nDone!\n";
}