#include "hittable.h"
#include "rtweekend.h"

#include <iostream>
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

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
    const int channels = 3;
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1080;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1.0), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1.0), 100.0));

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * aspect_ratio;
    auto focal_length = 1.0;

    auto origin = point3(0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2.0 - vertical/2.0 - vec3(0, 0, focal_length);

    // Render
    stbi_flip_vertically_on_write(true);
    std::unique_ptr<char[]> img = std::make_unique<char[]>(image_width * image_height * channels);

    for(int j = image_height - 1; j >= 0 ; --j) {
        std::cout << "\rScanlines remaining: " << j << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = static_cast<double>(i) / (image_width - 1);
            auto v = static_cast<double>(j) / (image_height - 1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r, world);

            img[(j * channels * image_width) + i*channels    ] = static_cast<char>(255.999 * pixel_color.x);
            img[(j * channels * image_width) + i*channels + 1] = static_cast<char>(255.999 * pixel_color.y);
            img[(j * channels * image_width) + i*channels + 2] = static_cast<char>(255.999 * pixel_color.z);
        }
    }

    stbi_write_png(result_path, image_width, image_height, channels, img.get(), image_width * channels);
    
    std::cout << "\nDone!\n";
}