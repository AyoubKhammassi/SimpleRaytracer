#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <iostream>
#include <cassert>

#pragma region vector
//one dimensional vector of dimeension DIM and components of type T
template <size_t DIM, typename T>
struct vec
{
    vec()
    {
        for (size_t i = DIM; i--; data[i] = T())
            ;
    }

    T &operator[](const size_t i)
    {
        assert(i < DIM);
        return data[i];
    }
    const T &operator[](const size_t i) const
    {
        assert(i < DIM);
        return data[i];
    }

    T data[DIM];
};

typedef vec<2, float> Vec2f;
typedef vec<2, int> Vec2i;
typedef vec<3, float> Vec3f;
typedef vec<3, int> Vec3i;
typedef vec<4, float> Vec4f;
typedef vec<4, int> Vec4i;

template <typename T>
struct vec<2, T>
{
    vec() : x(T()), y(T()) {}
    vec(T X, T Y) : x(X), y(Y) {}

    T x, y;
};

template <typename T>
struct vec<3, T>
{
    vec() : x(T()), y(T()), z(T()) {}
    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    T &operator[](const size_t i)
    {
        assert(i < 3);
        return i <= 0 ? x : (1 == i ? y : z);
    }
    const T &operator[](const size_t i) const
    {
        assert(i < 3);
        return i <= 0 ? x : (1 == i ? y : z);
    }
    float norm() { return std::sqrt(x * x + y * y + z * z); }
    vec<3, T> &normalize(T l = 1.f)
    {
        const float n = norm();
        const float ni = (l / n);
        *this = (*this) * ni;
        return *this;
    }

    T x, y, z;
};

//Vector Addition operator
template <size_t DIM, typename T>
vec<DIM, T> operator+(const vec<DIM, T> &l, const vec<DIM, T> &r)
{
    vec<DIM, T> ret;
    for (size_t index = DIM; index--; ret[index] = l[index] + r[index])
        ;
    return ret;
}

//Vector Substraction operator
template <size_t DIM, typename T>
vec<DIM, T> operator-(const vec<DIM, T> &l, const vec<DIM, T> &r)
{
    vec<DIM, T> ret;
    for (size_t index = DIM; index--; ret[index] = l[index] - r[index])
        ;
    return ret;
}

//this is the dot product, not a normal multiplication
template <size_t DIM, typename T>
T operator*(vec<DIM, T> &l, const vec<DIM, T> &r)
{
    T ret = T();
    for (size_t index = DIM; index--; ret += l[index] * r[index])
        ;
    return ret;
}

//this is the dot product, not a normal multiplication
template <size_t DIM, typename T>
T operator*(const vec<DIM, T> &l, const vec<DIM, T> &r)
{
    T ret = T();
    for (size_t index = DIM; index--; ret += l[index] * r[index])
        ;
    return ret;
}

//Vector product with a scaler
template <size_t DIM, typename T, typename S>
vec<DIM, T> operator*(const vec<DIM, T> &l, const S &s)
{
    vec<DIM, T> ret;
    for (size_t index = DIM; index--; ret[index] = l[index] * s)
        ;
    return ret;
}

//Multiplying the vector with a minus one to make it negatif
template <size_t DIM, typename T>
vec<DIM, T> operator-(vec<DIM, T> &r)
{
    return r * T(-1); //using the scalar product aleady defined above
}

//The cross product for vectors with three components
template <typename T>
vec<3, T> cross(const vec<3, T> &v1, const vec<3, T> &v2)
{
    return vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

//The cross product for vectors with three components
template <typename T>
vec<3, T> cross(vec<3, T> &v1, vec<3, T> &v2)
{
    return vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

template <size_t DIM, typename T>
std::ostream &operator<<(std::ostream &out, vec<DIM, T> &v)
{
    for (unsigned int i = 0; i < DIM; i++)
    {
        out << v[i] << " ";
    }
    return out;
}

#pragma endregion vector

#pragma region ray
struct Ray
{
    Vec3f ray_origin;
    Vec3f ray_direction;

    Ray(Vec3f ro, Vec3f rd) : ray_origin(ro), ray_direction(rd){};
};

struct hit_record
{
    double t;
    Vec3f point;
    Vec3f normal;
};

#pragma endregion ray

#pragma region shapes
//Geometry Shapes

struct Geometry
{
protected:
    Vec3f center;

public:
    //default constructor
    Geometry() = default;
    Geometry(const Vec3f &c) : center(c) {}
    virtual bool ray_intersect(const Ray &ray, float max_t, hit_record &rec) const { return true; }
    virtual Vec3f get_normal(const Vec3f p) const { return (p - center).normalize(); }
};

struct Sphere : public Geometry
{
private:
    float radius;

public:
    //constructor
    Sphere(const Vec3f &c, const float &r) : radius(r)
    {
        center = c;
    }

    virtual bool ray_intersect(const Ray &ray, float max_t, hit_record &rec) const
    {
        //the vector from the ray origin to the center of the sphere
        Vec3f distanceVec = (center - ray.ray_origin);

        //use the dot product to project the distanceVec on the ray direction vector : ProjectionTest
        float pt = ray.ray_direction * distanceVec;
        //std::cout << " Distance from sphere center to projection point: " << pt << "\n";

        //two cases depending on the value of pt

        if (pt <= 0.0f)
            return false; //the sphere is behind the ray

        //we need to find the distance between the sphere center and the projection point on the ray, using c^2 = a^2 + b^2 : DistanceToProjectionPoint dtpp
        float dtpp = (distanceVec * distanceVec) - pt * pt;

        if (dtpp > radius * radius)
            return false;
        else //intersection with the sphere in one or two points
        {
            //distance from projection pt to intersection point, it's the same in for both intersection points : dfpti
            float dfpti = sqrtf(radius * radius - dtpp);
            //first point of intersection
            float i0 = pt - dfpti;
            float i1 = pt + dfpti;
            const float t0 = (i0 <= 0.0f) ? i1 : i0;
            if (t0 > max_t)
                return false;

            rec.t = t0;
            rec.point = ray.ray_origin + ray.ray_direction * t0;
            rec.normal = get_normal(rec.point);
            return true;
        }
    }
};

#pragma endregion shapes

#endif // _GEOMETRY_H_