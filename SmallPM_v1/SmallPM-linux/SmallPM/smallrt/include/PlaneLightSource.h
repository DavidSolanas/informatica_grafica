/****************************************+
 * Fichero: PlaneLightSource.h
 * Autor: David Solanas Sanz    738630
 *****************************************/

#ifndef __PLANELIGHTSOURCE_H
#define __PLANELIGHTSOURCE_H

#include "LightSource.h"
#include "Plane.h"

class BoundedPlane : public Plane
{
public:
	/**
     * A               B
     * #################
     * #               #
     * #               #
     * #################
     * D               C
     */
	Vector3 A;
	Vector3 B;
	Vector3 C;
	Vector3 D;

	BoundedPlane(const Vector3 &_A, const Vector3 &_B, const Vector3 &_C, BSDF *mat);
	bool isInsidePlane(const Vector3 &p) const;
	bool intersect(Ray &r, Intersection &it, float time = 0.) const override;
	Vector3 generate_random_point_on_plane() const;
	Vector3 get_center() const override;
	Real get_area() const;
};

/** PlaneLightSource class. */
class PlaneLightSource : public LightSource
{
	BoundedPlane plane;

public:
	PlaneLightSource(World *_world,
					 const BoundedPlane &p,
					 const Vector3 &ints) : LightSource(_world, ints), plane(p) {}

	Vector3 get_position() const;
	Vector3 get_light_point() const;
	Vector3 get_plane_normal() const;
	Real get_area() const;
	//-----------------------------------------------------------------
	Vector3 get_incoming_direction(const Vector3 &point_lighted) const;
	Vector3 get_incoming_direction(const Vector3 &point_lighted, const Vector3 &light_point) const;
	Vector3 get_incoming_light(const Vector3 &point_lighted) const;
	Vector3 get_incoming_light(const Vector3 &point_lighted, const Vector3 &light_point) const;
	//-------------------------------------------------------------------
	bool is_visible(const Vector3 &point_lighted) const;
	bool is_visible(const Vector3 &point_lighted, const Vector3 &light_point) const;

	bool is_point_source() const;
	bool is_point_on_surface(const Vector3 &point) const;
	/** Number of samples to cast from a given Vector3 towards the light source. */
	int get_no_samples() const override;
};

#endif
