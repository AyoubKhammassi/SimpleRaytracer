#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <iostream>
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
        asset(i < DIM);
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
    vec<3, T> &normalize(T l = 1)
    {
        *this = (*this) * (l / norm());
        return *this;
    }

    T x, y, z;
};

//Vector Addition operator
template <size_t DIM, typename T>
vec<DIM, T> operator+(vec<DIM, T> &l, const vec<DIM, T> &r)
{
    for (size_t index = DIM; index--; l[index] += r[index])
        ;
    return l;
}

//Vector Substraction operator
template <size_t DIM, typename T>
vec<DIM, T> operator-(vec<DIM, T> &l, const vec<DIM, T> &r)
{
    for (size_t index = DIM; index--; l[index] -= r[index])
        ;
    return l;
}

//this is the dot product, not a normal product
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
vec<DIM, T> operator+(vec<DIM, T> &l, const S &s)
{
    for (size_t index = DIM; index--; l[index] *= s)
        ;
    return l;
}

//Multiplying the vector with a minus one to make it negatif
template <size_t DIM, typename T>
vec<DIM, T> operator-(vec<DIM, T> &r)
{
    return r * T(-1); //using the scalar product aleady defined above
}

//The cross product for vectors with three components
template <typename T>
vec<3, T> operator-(vec<3, T> &v1, vec<3, T> &v2)
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

#endif // _GEOMETRY_H_