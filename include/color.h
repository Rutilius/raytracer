#pragma once

#include "rtweekend.h"
#include <iostream>

constexpr void write_color(char img[], int index, const color &pixel_color, int sample_per_pixel) {
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / sample_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    img[index    ] = static_cast<char>(256 * clamp(r, 0.0, 0.999));
    img[index + 1] = static_cast<char>(256 * clamp(g, 0.0, 0.999));
    img[index + 2] = static_cast<char>(256 * clamp(b, 0.0, 0.999));
}