/****************************************+
 * Fichero: Path_tracer.cpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#include <iostream>
#include <cmath>
#include "Scene.hpp"
#include "Transformation.hpp"
#include <fstream>
#include <thread>
#include <ctime>
#include <cstring>
#include <random>
#include "Lambertian.hpp"
#include "Textures.hpp"

const int NUM_THREADS = 4;

std::ostream &operator<<(std::ostream &os, const RGB &c)
{
    os << c.r << " " << c.g << " " << c.b << "     ";
    return os;
}

RGB trace_path(const World &w, Ray &ray)
{
    ray.set_parameter(INFINITY);
    Ray ray_light = ray;
    Light *light = w.first_light_intersection(ray_light);
    Object *obj = w.first_intersection(ray);
    if (obj == nullptr && light == nullptr)
    {
        //No ha habido intersección
        return w.get_background();
    }
    // Ha intersectado con una fuente de luz, devuelve el color de dicha fuente
    if (light != nullptr && ray_light.get_parameter() <= ray.get_parameter())
    {
        return light->get_light_amount();
    }

    Point hit = ray.get_position();
    Direction hit_normal = obj->getNormal(hit);

    Ray new_ray;

    RGB brdf_g_pdf = obj->get_material()->get_outgoing_sample_ray(ray, hit_normal, new_ray);

    if (brdf_g_pdf == -1)
    {
        //Rayo muerto por ruleta rusa
        return w.get_background();
    }

    //Direct light contribution SOLO DE LAS PUNTUALES (omitir por ahora)
    RGB total_direct_point_light_cont = w.get_incoming_light(hit, hit_normal);

    RGB Li = trace_path(w, new_ray);
    Li = Li + total_direct_point_light_cont;

    if (obj->has_texture())
    {
        float u, v;
        obj->get_uv(hit, hit_normal, u, v);
        return Li * brdf_g_pdf * get_pixel(obj->data, u, v);
    }

    return Li * brdf_g_pdf;
}

void render_image(std::vector<std::vector<RGB>> &data, const int numSamples, const World &w,
                  const Camera &c, const int H, const int W,
                  const int inicio_x, const int fin_x, const int inicio_y, const int fin_y)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int y = inicio_y; y < fin_y; y++)
    {
        for (int x = inicio_x; x < fin_x; x++)
        {
            RGB color(0, 0, 0);
            for (int k = 0; k < numSamples; k++)
            {
                float xrand = dist(mt), yrand = dist(mt);
                Point pixel(x + xrand, y + yrand, c.f.mod());
                Direction d_ray = normalize(pixel - c.o);
                Ray ray(c.o, d_ray);
                color = color + trace_path(w, ray);
            }
            color = color / numSamples;
            data[y][x] = color;
        }
    }
}

void store_hdr(const std::vector<std::vector<RGB>> &data, std::string filename, const int H, const int W, const int cr)
{
    std::ofstream f(filename);
    if (f.is_open())
    {
        f << "P3" << std::endl;
        f << W << " " << H << std::endl;
        f << cr << std::endl;
        for (int i = data.size() - 1; i >= 0; i--)
        {
            for (RGB color : data[i])
            {
                f << color;
            }
            f << "\n";
        }
        f.close();
    }
}

int main(int argc, char const *argv[])
{

    std::string usage = "Usage: Path_tracer <file> -n ppp";
    std::string filename;
    if (argc != 4)
    {
        std::cerr << "Incorrect number of parameters" << std::endl;
        std::cerr << usage << std::endl;
        exit(1);
    }
    else
    {
        int ppp = 1;
        for (int i = 1; i < argc; i++)
        {
            switch (i)
            {
            case 1:
                filename = argv[i];
                break;
            case 2:
                if (strcmp("-n", argv[i]) != 0)
                {
                    std::cerr << "Incorrect parameter, found \"" << argv[i] << "\" but expected \"-n\"" << std::endl;
                    std::cerr << usage << std::endl;
                    exit(1);
                }
                break;
            case 3:
                ppp = atoi(argv[i]);
                break;

            default:
                break;
            }
        }
        const int H = 480, W = 640;
        BRDF *white = new Lambertian(RGB(.85, .85, .85));
        std::cout << "Loading scene..." << std::endl;
        Direction l((int)W / 2, 0, 0);
        Direction u(0, (int)H / 2, 0);
        Direction f(0, 0, u.mod() / tan(M_PI / 24));
        Point c0((int)W / 2, (int)H / 2, 0);
        Camera c(f, u, l, c0);
        std::vector<Object *> objs = scene5(c, W, H);
        PlaneLight lightA(
            BoundedPlane(
                Point(W / 2 - 150, H, c.f.mod() + 900),
                Point(W / 2 - 150, H, c.f.mod() + 800),
                Point(W / 2 - 50, H, c.f.mod() + 800),
                Point(W / 2 - 50, H, c.f.mod() + 900),
                white),
            36000, RGB(1., 1., 1.));

        PlaneLight lightB(
            BoundedPlane(
                Point(W / 2 + 50, H, c.f.mod() + 900),
                Point(W / 2 + 50, H, c.f.mod() + 800),
                Point(W / 2 + 150, H, c.f.mod() + 800),
                Point(W / 2 + 150, H, c.f.mod() + 900),
                white),
            36000, RGB(1., 1., 1.));

        PlaneLight lightC(
            BoundedPlane(
                Point(W / 2 - 150, H, c.f.mod() + 600),
                Point(W / 2 - 150, H, c.f.mod() + 500),
                Point(W / 2 - 50, H, c.f.mod() + 500),
                Point(W / 2 - 50, H, c.f.mod() + 600),
                white),
            36000, RGB(1., 1., 1.));

        PlaneLight lightD(
            BoundedPlane(
                Point(W / 2 + 50, H, c.f.mod() + 600),
                Point(W / 2 + 50, H, c.f.mod() + 500),
                Point(W / 2 + 150, H, c.f.mod() + 500),
                Point(W / 2 + 150, H, c.f.mod() + 600),
                white),
            36000, RGB(1., 1., 1.));

        PointLight light2(Point(W / 2, H - 50, c.f.mod() + 750), 36000000., RGB(1., 1., 1.));
        PointLight light3(Point(W / 2 + 150, H - 50, c.f.mod() + 375), 36000000., RGB(1., 1., 1.));
        World w;
        //w.add_light(&light3);
        w.add_light(&lightA);
        w.add_light(&lightB);
        w.add_light(&lightC);
        w.add_light(&lightD);
        w.add_objects(objs);
        w.set_background(RGB(0, 0, 0));
        std::thread P[NUM_THREADS];
        std::vector<std::vector<RGB>> img_data(H, std::vector<RGB>(W));
        int inicio_x = c.o.x - c.l.mod();
        int fin_x = c.o.x + c.l.mod();
        int inicio_y = c.o.y - c.u.mod();
        int fin_y = c.o.y + c.u.mod();
        int split_y = (fin_y - inicio_y) / NUM_THREADS;

        //Medir tiempo render
        struct timespec start, finish;
        clock_gettime(CLOCK_REALTIME, &start);
        std::cout << "Rendering image..." << std::endl;

        for (int i = 0; i < NUM_THREADS; i++)
        {
            P[i] = std::thread(&render_image, ref(img_data), ppp, w, c, H, W,
                               inicio_x,
                               fin_x,
                               inicio_y + i * split_y,
                               inicio_y + (i + 1) * split_y);
        }
        for (int i = 0; i < NUM_THREADS; i++)
        {
            P[i].join();
        }

        // Obtener tiempo de ejecución
        clock_gettime(CLOCK_REALTIME, &finish);
        double elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        std::cout << "Render completed in " << elapsed << " seconds, storing image..." << std::endl;

        // Guardar imagen
        store_hdr(img_data, filename, H, W, 255);
    }
    return 0;
}
