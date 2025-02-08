#ifndef _textures_h_included
#define _textures_h_included

#include "quickcg2.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

namespace Textures {
    void initPermutation();
    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
    double perlin(double x, double y, double z);
    std::vector<QuickCG::ColorRGB> getCloudTexture(int h, int w);
    std::vector<QuickCG::ColorRGB> getWaterTexture(int h, int w);
    std::vector<QuickCG::ColorRGB> getGrassTexture(int h, int w);
    std::vector<QuickCG::ColorRGB> getBrownGroundTexture(int h, int w);
}

#endif