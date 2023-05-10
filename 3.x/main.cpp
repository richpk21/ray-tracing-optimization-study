#include <iostream>
#include "vec3.h"
#include "color.h"

int main() {

	const int image_width = 256;
	const int image_height = 256;

	std::cout << "P3\n" << image_width << ' ' << image_height << " \n255\n";

	for (int j = image_height; j >= 0 ; j--) { 
		std::cerr << "\rScanline remaining :" << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			// auto r = double(i) / (image_width -1);
			// auto g = double(j) / (image_height -1);
			// auto b = 0.25;

			// int ir = static_cast<int>(255.999 * r);
			// int ig = static_cast<int>(255.999 * g);
			// int ib = static_cast<int>(255.999 * b);

			color pixel_color(double(i)/(image_width-1), double(j)/(image_height-1), 0.25);
			write_color(std::cout, pixel_color);
			//std::cout << ir << ' ' << ig << ' ' << ib << "\n";
		}
	}

	std::cerr << "\nDone\n"; 

}
