#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Geometry.h"

//cast ray
Vec3f cast_ray(const Vec3f &r_origin, const Vec3f &r_direction, const Sphere &sphere)
{
    float sphere_dist = std::numeric_limits<float>::max();
    if (sphere.ray_intersect(r_origin, r_direction, sphere_dist))
        return Vec3f(0.2, 0.7, 0.8);

    return Vec3f(0.4, 0.4, 0.3);
}
//Writes image to disk
void render(const Sphere &sphere)
{
    const int width = 1024;
    const int height = 768;
    const int fov = 60;
    const float ratio = width / (float)height;

    std::vector<Vec3f> frameBuffer(width * height);

    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * ratio;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
            Vec3f dir = Vec3f(x, y, -1).normalize();
            frameBuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), dir, sphere);
        }
    }

    std::ofstream ofs; // the output file stream used to save the FB to a file
    ofs.open("./out.ppm");
    ofs << "P3\n"
        << width << " " << height << "\n255\n";

    for (size_t i = 0; i < height * width; ++i)
    {
        //ofs << (int)(255 * frameBuffer[i][j]) << " "; // std::max(0.f, std::min(1.f, frameBuffer[i][j])));
        int ir = int(255.99 * frameBuffer[i][0]);
        int ig = int(255.99 * frameBuffer[i][1]);
        int ib = int(255.99 * frameBuffer[i][2]);
        ofs << ir << " " << ig << " " << ib << "\n";
    }
    ofs.close();
}

int main()
{
    Vec3f c(-3, 0, -16);
    Sphere s(c, 2.0f);
    render(s);
    return 0;
}