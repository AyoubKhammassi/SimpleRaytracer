#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Geometry.h"

#define M_PI 3.1415926535897932384626433832795028841971693993751058209

//cast ray
Vec3f cast_ray(const Ray &ray, const std::vector<Geometry *> &spheres, int &bounces)
{
    hit_record rec;
    bool found_hit = false;
    for (auto sphere : spheres)
    {
        if (sphere->ray_intersect(ray, 1000.0, rec))
        {
            found_hit = true;
            break;
        }
    }

    if (found_hit)
    {
        return Vec3f(1, 1, 1);
        //TODO: change geometry with meshes that contain geometry for intersection and materials for lighting calculation
    }
    else //gradient for the background
    {
        Vec3f unit_vec = ray.ray_direction;
        unit_vec.normalize();
        float t = 0.5 * (unit_vec.y + 1.0);
        return (Vec3f(1., 1., 1.) * (1.0f - t)) + (Vec3f(0.5, 0.7, 1.0) * t); //(0.4, 0.4, 0.3);
    }
}
//Writes image to disk
void render(const std::vector<Geometry *> &spheres)
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
            const Ray r(Vec3f(0, 0, 0), dir.normalize());
            int bounces = 0;
            frameBuffer[i + j * width] = cast_ray(r, spheres, bounces);
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
    std::vector<Geometry *> spheres;
    Sphere s1(Vec3f(0.0, 0., -50.), 2.0f);
    Sphere s2(Vec3f(20.0, 0., -20.), 7.0f);
    Sphere s3(Vec3f(-5.0, 0., -8.), 5.0f);
    spheres.push_back(&s1);
    spheres.push_back(&s2);
    spheres.push_back(&s3);

    render(spheres);
    return 0;
}