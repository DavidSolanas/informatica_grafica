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
#include "PhotonMapping.h"
#include "World.h"
#include "Intersection.h"
#include "Ray.h"
#include "BSDF.h"
#include "../smallrt/include/globals.h"
#include <random>

/**
 * This function returns a random real number between 
 * the values 'a' and 'b'
 */
Real get_random_value(const Real a, const Real b)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<Real> dist(a, b);
	return dist(mt);
}

//*********************************************************************
// Compute the photons by tracing the Ray 'r' from the light source
// through the scene, and by storing the intersections with matter
// in the lists 'xx_photons', storing the diffuse (global) and caustic
// photons respectively. For efficiency, both are computed at the same
// time, since computing them separately would result into a lost of
// several samples marked as caustic or diffuse.
// Same goes with the boolean 'direct', that specifies if direct
// photons (from light to surface) are being stored or not.
// The initial traced photon has energy defined by the tristimulus
// 'p', that accounts for the emitted power of the light source.
// The function will return true when there are more photons (caustic
// or diffuse) to be shot, and false otherwise.
//---------------------------------------------------------------------
bool PhotonMapping::trace_ray(const Ray &r, const Vector3 &p,
							  std::list<Photon> &global_photons, std::list<Photon> &caustic_photons, bool direct, bool direct_only)
{
#ifndef MAX_PHOTON_ITERATIONS
#define MAX_PHOTON_ITERATIONS 20
#endif

	//Check if max number of shots done...
	if (++m_nb_current_shots > m_max_nb_shots)
	{
		return false;
	}

	// Compute irradiance photon's energy
	Vector3 energy(p);

	Ray photon_ray(r);
	photon_ray.shift();

	bool is_caustic_particle = false;

	//Iterate the path
	while (1)
	{
		// Throw ray and update current_it
		Intersection it;
		world->first_intersection(photon_ray, it);

		if (!it.did_hit())
			break;

		//Check if has hit a delta material...
		if (it.intersected()->material()->is_delta())
		{
			// If delta material, then is caustic...
			// Don't store the photon!
			is_caustic_particle = true;
		}
		else if (photon_ray.get_level() > 0 || direct || direct_only)
		{
			//If non-delta material, store the photon!
			if (is_caustic_particle)
			{
				//If caustic particle, store in caustics
				if (caustic_photons.size() < m_nb_caustic_photons)
					caustic_photons.push_back(Photon(it.get_position(), photon_ray.get_direction(), energy));
			}
			else
			{
				//If non-caustic particle, store in global
				if (global_photons.size() < m_nb_global_photons)
					global_photons.push_back(Photon(it.get_position(), photon_ray.get_direction(), energy));
			}
			is_caustic_particle = false;
		}

		Real pdf;

		Vector3 surf_albedo = it.intersected()->material()->get_albedo(it);
		Real avg_surf_albedo = surf_albedo.avg();

		Real epsilon2 = get_random_value(0.f, 1.f);
		if (epsilon2 > avg_surf_albedo || photon_ray.get_level() > MAX_PHOTON_ITERATIONS)
			break;

		if (direct_only && !is_caustic_particle && photon_ray.get_level() > 1)
			break;

		// Random walk's next step
		// Get sampled direction plus pdf, and update attenuation
		it.intersected()->material()->get_outgoing_sample_ray(it, photon_ray, pdf);

		// Shade...
		energy = energy * surf_albedo;
		if (!it.intersected()->material()->is_delta())
			energy *= dot_abs(it.get_normal(), photon_ray.get_direction()) / 3.14159;

		energy = energy / (pdf * avg_surf_albedo);
	}

	if (caustic_photons.size() == m_nb_caustic_photons &&
		global_photons.size() == m_nb_global_photons)
	{
		m_max_nb_shots = m_nb_current_shots - 1;
		return false;
	}

	return true;
}

//*********************************************************************
// TODO: Implement the preprocess step of photon mapping,
// where the photons are traced through the scene. To do it,
// you need to follow these steps for each shoot:
//  1 - Sample a world's light source in the scene to create
//		the initial direct photon from the light source.
//	2 - Trace the photon through the scene storing the inter-
//		sections between the photons and matter. You can use
//		the function 'trace_ray' for this purpose.
//	3 - Finally, once all the photons have been shot, you'll
//		need to build the photon maps, that will be used later
//		for rendering.
//		NOTE: Careful with function
//---------------------------------------------------------------------
void PhotonMapping::preprocess()
{
	// De momento solo con una fuente de luz para probar
	bool go_on = true;
	std::list<Photon> global_photons, caustic_photons;
	bool direct = m_raytraced_direct, direct_only = false;
	while (go_on)
	{
		Real x, y, z;
		do
		{
			//Random unit vector <x,y,z>
			x = get_random_value(-1.f, 1.f);
			y = get_random_value(-1.f, 1.f);
			z = get_random_value(-1.f, 1.f);
		} while (x * x + y * y + z * z > 1);
		Vector3 d(x, y, z);
		Ray r(world->light(0).get_position(), d);
		go_on = trace_ray(r, world->light(0).get_intensities() / m_max_nb_shots, global_photons, caustic_photons, direct, direct_only);
	}
	//Fotones trazados
	//construir mapa de fotones
	for (auto &&photon : global_photons)
	{
		std::vector<float> pos(3);
		pos[0] = photon.position.data[0];
		pos[1] = photon.position.data[1];
		pos[2] = photon.position.data[2];
		m_global_map.store(pos, photon);
	}
	for (auto &&photon : caustic_photons)
	{
		std::vector<float> pos(3);
		pos[0] = photon.position.data[0];
		pos[1] = photon.position.data[1];
		pos[2] = photon.position.data[2];
		m_caustics_map.store(pos, photon);
	}
	if (global_photons.size() > 0)
		m_global_map.balance();
	if (caustic_photons.size() > 0)
		m_caustics_map.balance();
}

//*********************************************************************
// TODO: Implement the function that computes the rendering equation
// using radiance estimation with photon mapping, using the photon
// maps computed as a proprocess. Note that you will need to handle
// both direct and global illumination, together with recursive the
// recursive evaluation of delta materials. For an optimal implemen-
// tation you should be able to do it iteratively.
// In principle, the class is prepared to perform radiance estimation
// using k-nearest neighbors ('m_nb_photons') to define the bandwidth
// of the kernel.
//---------------------------------------------------------------------
Vector3 PhotonMapping::shade(Intersection &it0) const
{
	Vector3 L(0);
	Vector3 W(1);

	Intersection it(it0);

	//**********************************************************************
	// The following piece of code is included here for two reasons: first
	// it works as a 'hello world' code to check that everthing compiles
	// just fine, and second, to illustrate some of the functions that you
	// will need when doing the work. Goes without saying: remove the
	// pieces of code that you won't be using.
	//
	unsigned int debug_mode = 7;

	switch (debug_mode)
	{
	case 1:
		// ----------------------------------------------------------------
		// Display Albedo Only
		L = it.intersected()->material()->get_albedo(it);
		break;
	case 2:
		// ----------------------------------------------------------------
		// Display Normal Buffer
		L = it.get_normal();
		break;
	case 3:
		// ----------------------------------------------------------------
		// Display whether the material is specular (or refractive)
		L = Vector3(it.intersected()->material()->is_delta());
		break;

	case 4:
		// ----------------------------------------------------------------
		// Display incoming illumination from light(0)
		L = world->light(0).get_incoming_light(it.get_position());
		break;

	case 5:
		// ----------------------------------------------------------------
		// Display incoming direction from light(0)
		L = world->light(0).get_incoming_direction(it.get_position());
		break;

	case 6:
		// ----------------------------------------------------------------
		// Check Visibility from light(0)
		if (world->light(0).is_visible(it.get_position()))
			L = Vector3(1.);
		break;
	case 7:
		// ----------------------------------------------------------------
		// Reflect and refract until a diffuse surface is found, then show its albedo...
		int nb_bounces = 0;
		// MAX_NB_BOUNCES defined in ./SmallRT/include/globals.h
		while (it.intersected()->material()->is_delta() && ++nb_bounces < MAX_NB_BOUNCES)
		{
			Ray r;
			float pdf;
			it.intersected()->material()->get_outgoing_sample_ray(it, r, pdf);
			W = W * it.intersected()->material()->get_albedo(it) / pdf;

			r.shift();
			world->first_intersection(r, it);
		}
		std::vector<const KDTree<PhotonMapping::Photon, 3>::Node *> nodes;
		Real max_distance;
		std::vector<Real> pos(3);
		pos[0] = it.get_position().data[0];
		pos[1] = it.get_position().data[1];
		pos[2] = it.get_position().data[2];

		//Find 'm_nb_photons' global nearest to intersection it
		m_global_map.find(pos, m_nb_photons, nodes, max_distance);

		Real _area = M_PI * max_distance * max_distance;
		for (auto &&photon : nodes)
		{
			L = L + (it.intersected()->material()->get_albedo(it) * photon->data().flux);
		}
		L = L / _area;
		//std::cout << L.data[0] << "   " << L.data[1] << "   " << L.data[2] << std::endl;
		break;
	}
	// End of exampled code
	//**********************************************************************

	return L * W;
}