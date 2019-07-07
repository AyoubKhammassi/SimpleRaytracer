#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Geometry.h"

#define M_PI 3.1415926535897932384626433832795028841971693993751058209

//cast ray
Vec3f cast_ray(const Vec3f &r_origin, const Vec3f &r_direction, const Sphere &sphere)
{
    float sphere_dist = std::numeric_limits<float>::max();
    if (sphere.ray_intersect(r_origin, r_direction, sphere_dist))
        return Vec3f(0.4, 0.4, 0.3);

    return Vec3f(0.2, 0.7, 0.8);
}
//Writes image to disk
void render(const Sphere &sphere)
{
    const int width = 1024;
    const int height = 768;
    const float fov = M_PI / 2.;
    const float dir_z = -height / (2. * tan(fov / 2.));

    std::vector<Vec3f> frameBuffer(width * height);

    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            float dir_x = (i + 0.5) - width / 2.;
            float dir_y = -(j + 0.5) + height / 2.; // this flips the image at the same time

            Vec3f dir(dir_x, dir_y, dir_z);
            //std::cout << dir[2] << "\n";
            frameBuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), dir.normalize(), sphere);
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
    Vec3f c(0.0, 0., -10.);
    Sphere s(c, 2.0f);
    render(s);
    return 0;
}