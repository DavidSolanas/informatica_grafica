/****************************************+
 * Fichero: Path_tracer.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include <iostream>
#include "Geometry.hpp"
#include <cmath>
#include "Scene.hpp"
#include "Light.hpp"
#include "Transformation.hpp"
#include <random>
#include "BRDF.hpp"
#include <fstream>
#include <thread>
#include <ctime>

const int MAX_DEPTH = 3;
const int NUM_THREADS = 4;

std::ostream &operator<<(std::ostream &os, const RGB &c)
{
    os << c.r << " " << c.g << " " << c.b << "     ";
    return os;
}

Direction RandomUnitVectorInHemisphereOf(const Direction &n, const Point &p)
{
    //Cambio de coordenadas locales
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-1.f, 1.f);
    Matrix_Transformation T(n, p);
    float x = dist(mt), y = abs(dist(mt)), z = dist(mt);
    Direction _n(x, y, z);
    return normalize(T.inverse() * _n);
}

RGB trace_path(const World &w, Ray &ray, const int depth)
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
        return light->color * light->power;
    }

    Point hit = ray.get_position();
    Direction hit_normal = obj->getNormal(hit);
    Direction new_direction = RandomUnitVectorInHemisphereOf(hit_normal, hit);
    RGB color_brdf = obj->get_emission() * lambertian_BRDF(0.7f);

    const float p = 1 / (2 * M_PI);

    //Direct light contribution
    float total_direct_light_cont = w.get_incoming_light(hit, hit_normal);

    RGB direct_light = color_brdf * total_direct_light_cont / p;
    if (depth >= MAX_DEPTH)
    {
        // Ha habido intersección y rebote máximo
        return direct_light;
    }
    Ray new_ray(hit, new_direction);

    float cos_th = abs(dot(new_direction, hit_normal));

    RGB Li = trace_path(w, new_ray, depth + 1);

    return (color_brdf * (Li + direct_light) * cos_th / p);
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
                color = color + trace_path(w, ray, 0);
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

    std::string usage = "Usage: Path_tracer -n ppp";
    if (argc != 3)
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
                if (strcmp("-n", argv[i]) != 0)
                {
                    std::cerr << "Incorrect parameter, found \"" << argv[i] << "\" but expected \"-n\"" << std::endl;
                    std::cerr << usage << std::endl;
                    exit(1);
                }
                break;
            case 2:
                ppp = atoi(argv[i]);
                break;

            default:
                break;
            }
        }
        const int H = 720, W = 1280;

        Direction l((int)W / 2, 0, 0);
        Direction u(0, (int)H / 2, 0);
        Direction f(0, 0, u.mod() / tan(M_PI / 24));
        Point c0((int)W / 2, (int)H / 2, 0);
        Camera c(f, u, l, c0);
        std::vector<Object *> objs = cornell_box(c, W, H);
        PlaneLight light(
            BoundedPlane(
                Point(W / 2 - 150, H, c.f.mod() + 900),
                Point(W / 2 - 150, H, c.f.mod() + 600),
                Point(W / 2 + 150, H, c.f.mod() + 600),
                Point(W / 2 + 150, H, c.f.mod() + 900),
                RGB(255, 255, 255)),
            360000, RGB(255, 255, 255));
        World w;
        w.add_light(&light);
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
                               (i + 1 == NUM_THREADS) ? inicio_y + (i + 1) * split_y - 1 : inicio_y + (i + 1) * split_y);
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
        store_hdr(img_data, "/Users/david/Desktop/prueba2.ppm", H, W, 255);
    }
    return 0;
}
