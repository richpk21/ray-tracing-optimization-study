#include <iostream>
#include <chrono>
#include <thread>

#include "rt.h"

#if QLJS_HAVE_DEBUGBREAK
#define QLJS_CRASH_ALLOWING_CORE_DUMP() ::__debugbreak()
#elif QLJS_HAVE_BUILTIN_TRAP
#define QLJS_CRASH_ALLOWING_CORE_DUMP() __builtin_trap()
#else
#define QLJS_CRASH_ALLOWING_CORE_DUMP() ::std::abort()
#endif

color ray_color(const ray &r, const hittable &world, int depth)
{
	hit_record rec;
	if (depth <= 0)
	{
		return color(0, 0, 0);
	}

	// ray, tolerance of error close to point p., range, hit
	if (world.hit(r, 0.001, infinity, rec))
	{
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void simpleFunction()
{
	std::cerr << "simpleFunction\n";
}

void simpleFunction2()
{
	std::cerr << "simpleFunction\n";
}

void subrender(subRenderInfo renderInfo)
{

	// auto height = renderInfo.img_h_max - renderInfo.img_h_min;
	// auto width = renderInfo.img_w_max - renderInfo.img_w_min;

	for (int j = renderInfo.img_h_max - 1; j >= renderInfo.img_h_min; --j)
	{
		// std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = renderInfo.img_w_min; i < renderInfo.img_w_max; ++i)
		{
			color pixel_color(0, 0, 0);
			for (int s = 0; s < renderInfo.samples_per_pixel; ++s)
			{
				auto u = (i + random_double()) / (renderInfo.image_width - 1);
				auto v = (j + random_double()) / (renderInfo.image_height - 1);
				ray r = renderInfo.cam.get_ray(u, v);
				pixel_color += ray_color(r, renderInfo.world, renderInfo.max_depth);
			}

			// write_color(std::cout, pixel_color, renderInfo.samples_per_pixel);
			update_pixel(renderInfo, j, i, pixel_color);
		}
	}
}

int main()
{

	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	std::vector<color> image(image_height * image_width);
	// image.resize(image_height, std::vector<vec3>(image_width));

	// // sphere material
	// auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	// auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	// auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8));
	// auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2));

	// // World
	hittable_list world;
	// world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
	// world.add(make_shared<sphere>(point3(0, -100.5, -1), 0.5 , material_ground));

	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	// Camera
	camera cam;

	auto processor_count = std::thread::hardware_concurrency();
	std::cerr << "Number of cores: " << processor_count << "\n";

	// Render
	std::cout << "P3\n"
			  << image_width << " " << image_height << "\n255\n";

	// std::shared_ptr< std::vector<vec3>> image_ptr =

	subRenderInfo renderInfo = {
		.world = world,
		.cam = cam,
		.image = std::make_shared<std::vector<color>>(image),
		.image_height = image_height,
		.image_width = image_width,
		.img_w_min = 0,
		.img_w_max = image_width,
		.img_h_min = 0,
		.img_h_max = image_height,
		.max_depth = max_depth,
		.samples_per_pixel = samples_per_pixel,
	};


	processor_count = 7;
	int thread_count = processor_count;

	auto start = std::chrono::steady_clock::now();

	std::thread myThread[processor_count + 1];
	std::cerr << "processor count: " << processor_count << "\n";
	int steps = int(image_width / processor_count);
	int leftover = int(image_width % processor_count);

	for (int p = 0; p < processor_count; p++)
	{
		renderInfo.img_w_min = (steps)*p;
		renderInfo.img_w_max = (steps) * (p + 1);
		myThread[p] = std::thread(subrender, renderInfo);
		//}
	}

	if (0 < leftover)
	{
		std::cerr << "left over: " << leftover << "\n";
		thread_count++;
		renderInfo.img_w_min = (steps) * processor_count;
		renderInfo.img_w_max = (steps) * (processor_count + 1);
		myThread[thread_count-1] = std::thread(subrender, renderInfo);
	}
 
	for (int p = 0; p < thread_count; p++)
	{
		if (myThread[p].joinable())
			myThread[p].join();
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cerr << "Time it took to render: " << elapsed_seconds.count() << "s\n";

	write_image(std::cout, renderInfo);
	std::cerr << "\nDone.\n";
}
