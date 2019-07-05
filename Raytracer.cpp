#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Geometry.h"

//Writes image to disk
void render()
{
    const int width = 1024;
    const int height = 768;

    std::vector<Vec3f> frameBuffer(width * height);

    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            frameBuffer[i + j * width] = Vec3f(j / float(height), i / float(width), 0.0f);
        }
    }

    std::ofstream ofs; // the output file stream used to save the FB to a file
    ofs.open("./out.ppm");
    ofs << "P6\n"
        << width << " " << height << "\n255\n";

    for (size_t i = 0; i < height * width; ++i)
    {
        for (size_t j = 0; j < 3; j++)
        {
            ofs << (char)(255 * std::max(0.f, std::min(1.f, frameBuffer[i][j])));
        }
    }
    ofs.close();
}

int main()
{
    render();
    return 0;
}