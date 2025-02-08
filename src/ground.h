#ifndef GROUND_H
#define GROUND_H

#include <string>
#include <iostream>
#include <cmath>
#include "quickcg2.h"

using namespace QuickCG;

namespace Ground {
    void createCricketGround(std::vector<std::vector<Uint32> >& world);
    void drawCircle(std::vector<std::vector<Uint32> >& grid, int cx, int cy, int r, Uint32 symbol);
    void drawRectangle(std::vector<std::vector<Uint32> >& grid, int top, int left, int height, int width, Uint32 symbol);
}

#endif