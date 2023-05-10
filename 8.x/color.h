#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "rt.h"
#include <iostream>
// #include <cmath>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // gamma correction
    // color (r,g,b)^(1/gamma_value)
    // r = std::pow(r, 1/gamma_level);
    // in this case, gamma_value is 2.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    // write translated [0,255] value of each color component.
    out 
    << static_cast<int>(255.999 * clamp(r, 0.0, 0.999)) << ' '
    << static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << ' '
    << static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << '\n';
};

#endif