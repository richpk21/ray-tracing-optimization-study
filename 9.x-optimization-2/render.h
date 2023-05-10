#ifndef IMAGE_H
#define IMAGE_H

#include "color.h"
#include "hittable_list.h"
#include "camera.h"

struct subRenderInfo{
	hittable_list world; 
	camera cam;
	std::shared_ptr< std::vector<color>> image;
    const int image_height;
    const int image_width;
	int img_w_min;
	int img_w_max; 
	int img_h_min; 
	int img_h_max; 
	const int max_depth;
	const int samples_per_pixel;
};

void update_pixel(subRenderInfo renderInfo, int j, int i,  color pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // gamma correction
    // color (r,g,b)^(1/gamma_value)
    // r = std::pow(r, 1/gamma_level);
    // in this case, gamma_value is 2.
    auto scale = 1.0 / renderInfo.samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    // write translated [0,255] value of each color component.
    auto image_ptr = renderInfo.image;
    (*image_ptr)[(renderInfo.image_height-j)*renderInfo.image_width + i] = color(r,g,b);
}

void write_image(std::ostream &out, subRenderInfo renderInfo) {
    auto image_ptr = renderInfo.image;
    for (auto pixel: (*image_ptr)) {
    out 
    << static_cast<int>(255.999 * clamp(pixel.x(), 0.0, 0.999)) << ' '
    << static_cast<int>(255.999 * clamp(pixel.y(), 0.0, 0.999)) << ' '
    << static_cast<int>(255.999 * clamp(pixel.z(), 0.0, 0.999)) << '\n';
    }
};

#endif