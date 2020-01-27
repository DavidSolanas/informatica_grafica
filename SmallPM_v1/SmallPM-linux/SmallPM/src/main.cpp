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

#include <iostream>
#include <string.h>
#include "globals.h"

#include "World.h"
#include "RenderEngine.h"
#include "PhotonMapping.h"
#include "Film.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Mesh.h"
#include "BSDF.h"
#include "Phong.h"
#include "Specular.h"
#include "Transmissive.h"
#include "Texture.h"
#include "PlaneLightSource.h"

using namespace std;

namespace
{
Film *film;
RenderEngine *engine;
PhotonMapping *pm;
int sizex = 512, sizey = 512;
} // namespace

// main function - parse arguments and begin rendering.
int main(int argc, char *argv[])
{

	Real focal_distance = 2.6;

	string name_file = "name_file";

	unsigned int scene = 0;

	unsigned int photons_global = 10000,
				 photons_caustic = 10000,
				 max_shots = 20000,
				 nb_nearest_photons = 100;

	// ---------------------------------------------------------------------
	// Parse input
	for (int i = 1; i < argc; ++i)
	{
		if (!strcmp("-film-name", argv[i]))
		{
			++i;
			name_file = argv[i];
		}
		if (!strcmp("-film-size-x", argv[i]))
		{
			++i;
			sizex = atoi(argv[i]);
		}
		if (!strcmp("-film-size-y", argv[i]))
		{
			++i;
			sizey = atoi(argv[i]);
		}

		if (!strcmp("-scene", argv[i]))
		{
			++i;
			scene = atoi(argv[i]);
		}

		if (!strcmp("-pm-total-photons", argv[i]))
		{
			++i;
			photons_global = photons_caustic = atoi(argv[i]);
		}
		if (!strcmp("-pm-photons-global", argv[i]))
		{
			++i;
			photons_global = atoi(argv[i]);
		}
		if (!strcmp("-pm-photons-caustic", argv[i]))
		{
			++i;
			photons_caustic = atoi(argv[i]);
		}
		if (!strcmp("-pm-max-photons-shot", argv[i]))
		{
			++i;
			max_shots = atoi(argv[i]);
		}
		if (!strcmp("-pm-nb-nearest-neighbor", argv[i]))
		{
			++i;
			nb_nearest_photons = atoi(argv[i]);
		}
	}

	max_shots = (max_shots > (photons_global + photons_caustic)) ? max_shots : (photons_global + photons_caustic);

	// ----------------------------------------------------------------------
	// Set up viewing parameters and create a camera.
	//
	Vector3 camera_position(0, 1, 3.0);
	Vector3 looking_at(0, 1, 0);
	Vector3 up(0, 1, 0);
	Real view_plane_dist = 1;
	Camera camera(camera_position, looking_at, up, view_plane_dist);

	// ----------------------------------------------------------------------
	// Create a world
	//
	World *w = new World;

	w->set_ambient(Vector3(0, 0, 0));
	w->set_background(Vector3(0, 0, 0));

	BSDF *glass = new Transmissive(w, 1.5);
	BSDF *water = new Transmissive(w, 1.333);
	BSDF *diamond = new Transmissive(w, 2.42);
	BSDF *test = new Transmissive(w, 1.2);
	BSDF *mirror = new Specular(w);

	BSDF *white = new Lambertian(w, Vector3(.85, .85, .85));
	BSDF *red = new Lambertian(w, Vector3(.85, .085, .085));
	BSDF *blue = new Lambertian(w, Vector3(.085, .085, .85));
	BSDF *orange_phong = new Phong(w, Vector3(.425, .3, .0), 8.f);
	BSDF *grey_phong = new Phong(w);
	BSDF *green = new Lambertian(w, Vector3(.085, .85, .085));
	BSDF *orange = new Lambertian(w, Vector3(.85, .6, .0));
	BSDF *death = new Texture(w, Vector3(.085, .85, .085), "data/textures/death.ppm");

	Triangle *floor1 = new Triangle(Vector3(-1.5, 0, 1.5), Vector3(1.5, 0., 1.5),
									Vector3(-1.5, 0., -1.5), white);
	w->add_object(floor1);
	Triangle *floor2 = new Triangle(Vector3(-1.5, 0, -1.5), Vector3(1.5, 0., 1.5),
									Vector3(1.5, 0., -1.5), white);
	w->add_object(floor2);

	Triangle *ceil1 = new Triangle(Vector3(1.5, 2., 1.5), Vector3(-1.5, 2, 1.5),
								   Vector3(-1.5, 2., -1.5), white);
	w->add_object(ceil1);
	Triangle *ceil2 = new Triangle(Vector3(1.5, 2., 1.5), Vector3(-1.5, 2, -1.5),
								   Vector3(1.5, 2., -1.5), white);
	w->add_object(ceil2);

	Triangle *back1 = new Triangle(Vector3(1.5, 2.5, -1), Vector3(-1.5, 2.5, -1),
								   Vector3(-1.5, -.5, -1), white);
	w->add_object(back1);
	Triangle *back2 = new Triangle(Vector3(1.5, 2.5, -1), Vector3(-1.5, -.5, -1),
								   Vector3(1.5, -.5, -1), white);
	w->add_object(back2);

	Triangle *left1 = new Triangle(Vector3(-1, 2.5, -1.5), Vector3(-1, 2.5, 1.5),
								   Vector3(-1, -0.5, -1.5), red);
	w->add_object(left1);
	Triangle *left2 = new Triangle(Vector3(-1, 0, -1.5), Vector3(-1, 2.5, 1.5),
								   Vector3(-1, -.5, 1.5), red);
	w->add_object(left2);

	Object3D *right1 = new Triangle(Vector3(1, 2.5, 1.5), Vector3(1, 2.5, -1.5),
									Vector3(1, -.5, -1.5), blue);
	w->add_object(right1);
	Object3D *right2 = new Triangle(Vector3(1, 2.5, 1.5), Vector3(1, -.5, -1.5),
									Vector3(1, -.5, 1.5), blue);
	w->add_object(right2);

	switch (scene)
	{
	case 1:
	{
		Object3D *sphere1 = new Sphere(Vector3(0.45, .65, .4), 0.3, glass);
		w->add_object(sphere1);

		Object3D *sphere2 = new Sphere(Vector3(-0.31, 0.3, .0), 0.3, mirror);
		w->add_object(sphere2);

		Object3D *sphere3 = new Sphere(Vector3(-0.5, 1.3, -0.6), 0.3, blue);
		w->add_object(sphere3);
	}
	break;
	case 2:
	{
		Object3D *sphere1 = new Sphere(Vector3(0.5, 0.3, .5), 0.3, white);
		w->add_object(sphere1);

		Object3D *sphere2 = new Sphere(Vector3(-0.5, 0.5, 1.5), 0.3, red);
		w->add_object(sphere2);

		Object3D *sphere3 = new Sphere(Vector3(0., 0.3, .0), 0.3, white);
		w->add_object(sphere3);
	}
	break;
	case 3:
	{
		Object3D *sphere1 = new Sphere(Vector3(0.65, 1.2, -.65), 0.3, mirror);
		w->add_object(sphere1);

		Mesh *bunny = new Mesh("data/glass_win.obj", glass);
		w->add_object(bunny);
	}
	break;
	case 4:
	{
		Object3D *sphere1 = new Sphere(Vector3(0, 0.8, 0), 0.1, green);
		w->add_object(sphere1);
		Object3D *sphere2 = new Sphere(Vector3(0, 0.8, 0), 0.3, orange);
		w->add_object(sphere2);
	}
	break;
	case 5:
	{
		Object3D *sphere1 = new Sphere(Vector3(0.5, 0.6, -0.5), 0.2, orange);
		w->add_object(sphere1);

		Object3D *sphere2 = new Sphere(Vector3(0.5, 0.2, 0.3), 0.1, orange_phong);
		w->add_object(sphere2);

		Object3D *sphere3 = new Sphere(Vector3(-0.6, 0.4, 0.8), 0.15, blue);
		w->add_object(sphere3);

		Object3D *sphere4 = new Sphere(Vector3(0, 0.9, 0), 0.4, glass);
		w->add_object(sphere4);
	}
	break;
	case 6:
	{
		Mesh *monkey = new Mesh("data/death.obj", grey_phong);
		w->add_object(monkey);
	}
	break;
	default:
	{
		Object3D *sphere1 = new Sphere(Vector3(0.15, 0.4, .5), 0.3, water);
		w->add_object(sphere1);

		Object3D *sphere2 = new Sphere(Vector3(-0.5, 0.5, -.3), 0.3, diamond);
		w->add_object(sphere2);

		Object3D *sphere3 = new Sphere(Vector3(0.5, 1, -.3), 0.3, test);
		w->add_object(sphere3);
	}
	}

	BoundedPlane bp(Vector3(-.4, 2., -.3), Vector3(.4, 2., -.3), Vector3(.4, 2., .3), white);
	PlaneLightSource *pls = new PlaneLightSource(w, bp, Vector3(6, 6, 6));
	//LightSource *ls = new PointLightSource(w, Vector3(0., 1.9, 0), Vector3(5, 5, 5));
	w->add_light(pls);
	//LightSource *ls2 = new PointLightSource(w, Vector3(-0.5, 1.5, 0), Vector3(1.5, 1.5, 1.5));
	//LightSource *ls3 = new PointLightSource(w, Vector3(0.5, 1.5, 0), Vector3(1.5, 1.5, 1.5));
	//LightSource *ls4 = new PointLightSource(w, Vector3(0, 1, 2.7), Vector3(3, 3, 3));
	//w->add_light(ls2);
	//w->add_light(ls3);
	//w->add_light(ls4);

	w->fix();
	// ----------------------------------------------------------------------
	// Create Film and rendering engine
	//
	film = new Film(sizex, sizey);
	pm = new PhotonMapping(w, photons_global, photons_caustic, max_shots, nb_nearest_photons);
	engine = new RenderEngine(w, film, &camera, pm);

	engine->render(name_file);
}
