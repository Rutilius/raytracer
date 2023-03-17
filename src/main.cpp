#include "color.h"
#include "hittable.h"
#include "rtweekend.h"

#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iterator>
#include <functional>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <time.h>
#include <sstream>
#include <iomanip>   

#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"
#include "camera.h"
#include "materials.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

using std::make_shared;
using namespace std::chrono_literals;
 

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

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4.0, 0.2, 0.0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vec3() * random_vec3();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_vec3(0.5, 1.0);
                    auto fuzz = random_double(0.0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

struct render_data {
    unsigned int width;
    unsigned int height;

    unsigned int sample_per_pixel;
    unsigned int max_depth;

    camera cam;
    hittable_list world;

    render_data(
                const unsigned int width,
                const unsigned int height,
                const unsigned int sample_per_pixel, 
                const unsigned int max_depth, 
                const camera &camera, 
                const hittable_list &world
            ) : 
        width(width),
        height(height),
        sample_per_pixel(sample_per_pixel),
        max_depth(max_depth),
        cam(camera),
        world(world)
    { }
};

struct render_chunk {
    int start_x;
    int end_x;

    int start_y;
    int end_y;

    render_chunk(unsigned int start_x, unsigned int end_x, unsigned int start_y, unsigned int end_y) : start_x(start_x), end_x(end_x), start_y(start_y), end_y(end_y) {}
};

std::atomic<unsigned long> rows_progression(0);

void render(unsigned char img[], const int channels, const render_chunk &chunk, const render_data &data) {
    for(int j = chunk.end_y - 1; j >= chunk.start_y; --j) {
        for (int i = chunk.start_x; i < chunk.end_x; ++i) {
            color pixel_color(0);
            
            for (unsigned int s = 0; s < data.sample_per_pixel; ++s) {
                auto u = (i + random_double()) / (data.width - 1);
                auto v = (j + random_double()) / (data.height - 1);
                ray r = data.cam.get_ray(u, v);
                pixel_color += ray_color(r, data.world, data.max_depth);
            }

            auto start_color_index((j * channels * data.width) + i*channels);
            write_color(img, start_color_index, pixel_color, data.sample_per_pixel);
        }
        rows_progression.fetch_add(1);
        // std::cout << "Rendered line: " << j << ", column: [" << chunk.start_x << ", " << chunk.end_x << "]\n";
    }
}

// Copyright (c) 2011 Bonelli Nicola <bonelli@antifork.org> (GPL v2 licensed)
static unsigned int hardware_concurrency() {
    auto proc = []() -> int {
        std::ifstream cpuinfo("/proc/cpuinfo");
        return std::count(std::istream_iterator<std::string>(cpuinfo),
                        std::istream_iterator<std::string>(),
                        std::string("processor"));
    };

    auto hc = std::thread::hardware_concurrency();
    return hc ? hc : proc();
}

void show_progression(unsigned long total_rows) {
    std::thread([total_rows]() {
        do {
            std::cout.precision(2);
            std::cout.setf(std::ios::fixed, std::ios::floatfield);
            std::cout << "\rProgression: " << ((double)rows_progression / total_rows) * 100 << "%" << std::flush;
            std::this_thread::sleep_for(100ms);
        } while (rows_progression < total_rows);
        std::cout << "\rProgression: 100%" << std::flush;
    }).detach();
}

int main(int, char**) {
    //srand(time(NULL));
    const char *result_path = "result.png";

    // Image
    constexpr auto aspect_ratio = 3.0 / 2.0;
    constexpr int image_width = 1200;
    constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
    constexpr int sample_per_pixel = 100;
    constexpr int max_depth = 50;

    // World
    const auto world = random_scene();

    // Camera
    constexpr point3 lookfrom(13.0, 2.0, 3.0);
    constexpr point3 lookat(0.0);
    constexpr vec3 vup(0.0, 1.0, 0.0);
    constexpr auto depth_of_field = 10.0;
    constexpr auto aperture = 0.1;

    const camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, depth_of_field);

    // Render
    const auto thread_count = hardware_concurrency();
    std::cout << "Number of thread: " << thread_count << "\n";

    constexpr int channels = 3;
    constexpr int pixels_channels_quantity = image_width * image_height * channels;
    stbi_flip_vertically_on_write(true);
    auto img = std::make_unique<unsigned char[]>(pixels_channels_quantity);

    rows_progression = 0;

    if(thread_count > 0) {
        // Multithread approach
        auto threads = std::make_unique<std::thread[]>(thread_count);
        auto img_variations = std::make_unique<std::unique_ptr<unsigned char[]>[]>(thread_count);
        const render_data data(image_width, image_height, sample_per_pixel / thread_count, max_depth, cam, world);
                    
        show_progression(image_height * thread_count);

        for (unsigned int i = 0; i < thread_count; ++i) {
            img_variations[i] = std::make_unique<unsigned char[]>(pixels_channels_quantity);
            threads[i] = std::thread(render, img_variations[i].get(), channels, render_chunk(0, image_width, 0, image_height), std::ref(data));
        }

        for (unsigned int i = 0; i < thread_count; ++i) {
            threads[i].join();
        }

        // Save intermediate
        {
            const int thread_digits(floor(log10(thread_count) + 1)); 
            for (unsigned int i = 0; i < thread_count; ++i) {
                std::stringstream ss;
                ss << "intermediate_" << std::setw(thread_digits) << std::setfill('0') << (i + 1) << ".png";
                stbi_write_png(ss.str().c_str(), image_width, image_height, channels, img_variations[i].get(), image_width * channels);
            }
        }

        for (unsigned int i = 0; i < pixels_channels_quantity; ++i) {
            unsigned int channel_value{0};
            for (unsigned int j = 0; j < thread_count; ++j) {
                channel_value += img_variations[j][i];
            }
            
            unsigned int value = channel_value / thread_count;
            value = clamp(value, 0U, 256U);
            img[i] = static_cast<unsigned char>(value);
        }

    } else {
        // Singlethread approach
        const render_data data(image_width, image_height, sample_per_pixel, max_depth, cam, world);
        show_progression(image_height);
        render(img.get(), channels, render_chunk(0, image_width, 0, image_height), data);
    }

    /*
    for(uint j = image_height - 1; j >= 0 ; --j) {
        std::cout << "\rScanlines remaining: " << j << std::flush;
        for (uint i = 0; i < image_width; ++i) {
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
    */

    stbi_write_png(result_path, image_width, image_height, channels, img.get(), image_width * channels);
    
    std::cout << "\nDone!\n";
}