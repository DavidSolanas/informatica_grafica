#include "../smallrt/include/PlaneLightSource.h"
#include <random>
#include "../smallrt/include/World.h"
#include <iostream>

/**
 * This function returns a random real number between 
 * the values 'a' and 'b'
 */
Real get_random_val(const Real a, const Real b)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<Real> dist(a, b);
    return dist(mt);
}

BoundedPlane::BoundedPlane(const Vector3 &_A, const Vector3 &_B,
                           const Vector3 &_C, BSDF *mat) : Plane(_A, normalize(cross(_B - _A, _C - _A)), mat)
{
    A = _A;
    B = _B;
    C = _C;
    // D = B + (BA + BC)
    D = _B + ((_A - _B) + (_C - _B));
}

/**
 * D = (x2 - x1) * (yp - y1) - (xp - x1) * (y2 - y1)
 */
bool BoundedPlane::isInsidePlane(const Vector3 &p) const
{
    float s1 = dot(cross(B - A, p - A), this->get_normal(p));
    float s2 = dot(cross(C - B, p - B), this->get_normal(p));
    float s3 = dot(cross(D - C, p - C), this->get_normal(p));
    float s4 = dot(cross(A - D, p - D), this->get_normal(p));

    if (s1 < 0 || s2 < 0 || s3 < 0 || s4 < 0)
    {
        return false;
    }
    return true;
}

bool BoundedPlane::intersect(Ray &r, Intersection &it, float time) const
{
    bool intersect_plane = Plane::intersect(r, it, time);
    if (intersect_plane)
    {
        return isInsidePlane(r.get_position());
    }
    return false;
}

Vector3 BoundedPlane::generate_random_point_on_plane() const
{
    Vector3 ab = B - A;
    Vector3 ad = D - A;
    Real b = get_random_val(0., 1.), d = get_random_val(0., 1.);
    while (!isInsidePlane(A + (ab * b) + (ad * d)))
    {
        b = get_random_val(0., 1.), d = get_random_val(0., 1.);
    }
    return A + (ab * b + ad * d);
}

Vector3 BoundedPlane::get_center() const
{
    Vector3 ab = (B - A) / 2;
    Vector3 ad = (D - A) / 2;
    return A + ab + ad;
}

Real BoundedPlane::get_area() const
{
    return (B - A).length() * (D - A).length();
}

Vector3 PlaneLightSource::get_position() const
{
    return plane.get_center();
}

Vector3 PlaneLightSource::get_light_point() const
{
    return plane.generate_random_point_on_plane();
}

Vector3 PlaneLightSource::get_plane_normal() const
{
    return plane.get_normal(plane.get_center()).normalize();
}

Real PlaneLightSource::get_area() const
{
    return plane.get_area();
}

//-----------------------------------------------------------------
Vector3 PlaneLightSource::get_incoming_direction(const Vector3 &point_lighted) const
{
    return Vector3(0);
}

Vector3 PlaneLightSource::get_incoming_direction(const Vector3 &point_lighted, const Vector3 &light_point) const
{
    return normalize(point_lighted - light_point);
}

Vector3 PlaneLightSource::get_incoming_light(const Vector3 &point_lighted) const
{
    return Vector3(0);
}

Vector3 PlaneLightSource::get_incoming_light(const Vector3 &point_lighted, const Vector3 &light_point) const
{
    Vector3 wi = light_point - point_lighted;
    Real dist_2 = wi.length2();
    return intensities / dist_2;
}

bool PlaneLightSource::is_visible(const Vector3 &point_lighted, const Vector3 &light_point) const
{
    Vector3 d = normalize(light_point - point_lighted);
    Real dist_point_light = (light_point - point_lighted).length();
    Ray r(point_lighted, d);
    Intersection it;
    world->first_intersection(r, it);
    if (dot(d, get_plane_normal()) < 0 && (it.intersected() == nullptr || dist_point_light <= r.get_parameter()))
    {
        // Light source visible from point_lighted
        return true;
    }
    return false;
}

//-------------------------------------------------------------------
bool PlaneLightSource::is_visible(const Vector3 &point_lighted) const
{
    return false;
}

bool PlaneLightSource::is_point_source() const
{
    return false;
}

/** Number of samples to cast from a given Vector3 towards the light source. */
int PlaneLightSource::get_no_samples() const
{
    return 1000;
}

bool PlaneLightSource::is_point_on_surface(const Vector3 &point) const
{
    Vector3 pl = plane.generate_random_point_on_plane();
    bool on_surface = dot_abs(normalize(point - pl), get_plane_normal()) < SMALLEST_DIST;
    return on_surface && plane.isInsidePlane(point);
}
