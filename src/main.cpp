#include "color.h"
#include "hittable.h"
#include "rtweekend.h"

#include <iostream>
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"
#include "camera.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

using std::make_shared;


color ray_color(const ray &r, const hittable &world) {
    hit_record rec;
    if(world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1.0));
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

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1.0), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1.0), 100.0));

    // Camera
    camera cam(aspect_ratio);

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
                pixel_color += ray_color(r, world);
            }

            auto start_color_index((j * channels * image_width) + i*channels);
            write_color(img.get(), start_color_index, pixel_color, sample_per_pixel);
        }
    }

    stbi_write_png(result_path, image_width, image_height, channels, img.get(), image_width * channels);
    
    std::cout << "\nDone!\n";
}