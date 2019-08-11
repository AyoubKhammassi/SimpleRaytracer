#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Geometry.h"
#include <cmath>

// return a random double in the range [0.0,1.0)
double drand48()
{
    return rand() / (RAND_MAX + 1.0);
}

//Materials base class
struct Material
{
protected:
    Vec3f mainColor;

public:
    Material() = default;
    Material(Vec3f c) : mainColor(c) {}
    virtual Vec4f process() {}
};

//Diffuse material where light bounces in a random direction off the surface of intersection, OR gets absorbed
struct Diffuse : public Material
{
};

Vec3f random_in_unit_sphere(Geometry *geo, Vec3f p)
{
    //get the normal of the object geo on the point p on its surface
    Vec3f normal = geo->get_normal(p);
    Vec3f r = p + normal + Vec3f(drand48(), drand48(), drand48());

    //random point in unit sphere
    return r;
}
#endif