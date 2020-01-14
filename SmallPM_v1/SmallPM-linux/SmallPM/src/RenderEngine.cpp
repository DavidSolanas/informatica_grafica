/*********************************************************************************
Copyright (C) 2014 Adrian Jarabo (ajarabo@unizar.es)
Copyright (C) 2014 Diego Gutierrez (diegog@unizar.es)
All rights reserved.

This is an educational Ray Tracer developed for the course 'Informatica Grafica'
(Computer Graphics) tought at Universidad de Zaragoza (Spain). As such, it does not 
intend to be fast or general, but just to provide an educational tool for undergraduate
students. 

This software is provided as is, and any express or implied warranties are disclaimed.
In no event shall copyright holders be liable for any damage.
**********************************************************************************/

#ifdef WIN32
#include <windows.h>
#endif

#include "Timer.h"
#include "RenderEngine.h"
#include "Film.h"
#include <math.h>
#include <iostream>
#include <thread>

using namespace std;

const Vector3 RenderEngine::trace_ray(const Vector2 &pi)
{
	// NOTE: Vector2 'pi' stores the pixel coordinates in the range
	//			pi[0] in [0, film->sizex-1], pi[1] in [0, film->sizey-1]
	Vector2 p = film->window_coords2image_coords(pi);
	Ray r = camera->get_ray(p);

	Intersection it;
	world->first_intersection(r, it);

	// Get the ray's first intersection in the scene (if exists)
	if (it.did_hit())
	{
		return photon_mapping->shade(it);
	}
	else
		// ... or return background's color if the ray does not intersect.
		return world->get_background();
}

void RenderEngine::render_multi_thread(int x_start, int x_end, int y_start, int y_end)
{
	for (int y = y_start; y < y_end; ++y)
	{
		for (int x = x_start; x < x_end; ++x)
		{
			//Get color!
			Vector3 col = trace_ray(Vector2(x, y));

			//Store the RGB data on film
			film->draw_pixel(x, y, col);
		}
	}
}

void RenderEngine::render(const std::string &name)
{
	Timer timer;
	timer.start();

	cout << "Preparing scene to be rendered...\r";
	world->fix();
	{
		Real secs = timer.get_secs();
		int hours = static_cast<int>(secs) / 3600;
		secs -= hours * 3600;
		int minutes = static_cast<int>(secs) / 60;
		secs -= minutes * 60;
		std::cout << "Prepared scene to render: \t[" << hours << ":" << minutes << ":" << secs << "]             \n";
	}

	// ----------------------------------------------------------------------
	// Precompute...
	cout << "Shooting Photons...\r";
	timer.start();
	photon_mapping->preprocess();
	{
		Real secs = timer.get_secs();
		int hours = static_cast<int>(secs) / 3600;
		secs -= hours * 3600;
		int minutes = static_cast<int>(secs) / 60;
		secs -= minutes * 60;
		std::cout << "Photons Shot: \t[" << hours << ":" << minutes << ":" << secs << "]             \n";
	}

	// ----------------------------------------------------------------------
	// Start timer and go ...
	//
	int x = 0;
	Real weight = 1. / (NB_SAMPLES_ANTIALIASING);
	int sq_samples_aa = sqrtf(static_cast<float>(NB_SAMPLES_ANTIALIASING));

	cout << "Rendering ...\r";
	timer.start();

	// ----------------------------------------------------------------------
	//Raytrace all samples in the image

	//for all pixels...
	//trace a ray on each pixel (see function 'RenderEngine::trace_ray()')
	const int NUM_THREADS = 4;
	std::thread P[NUM_THREADS];
	int split_y = film->get_height() / NUM_THREADS;
	for (int i = 0; i < NUM_THREADS; i++)
	{
		P[i] = std::thread(&RenderEngine::render_multi_thread, this, 0, film->get_width(), i * split_y, (i + 1) * split_y);
	}
	for (int i = 0; i < NUM_THREADS; i++)
	{
		P[i].join();
	}

	// ----------------------------------------------------------------------
	// Rendering finished...
	Real secs = timer.get_secs();
	int hours = static_cast<int>(secs) / 3600;
	secs -= hours * 3600;
	int minutes = static_cast<int>(secs) / 60;
	secs -= minutes * 60;
	cout << "Rendering ..."
		 << "[DONE]: \t[" << hours << ":" << minutes << ":" << secs << "]                        "
		 << "\n";

	//..., save the image!
	film->write(name.c_str());
}
