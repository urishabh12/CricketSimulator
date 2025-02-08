// ball.h
#ifndef BALL_H
#define BALL_H

#include <string>
#include <iostream>
#include <cmath>
#include "quickcg2.h"

class Ball {
public:
    int currX;
    int currY;
    int currZ;
    int height;
    int width;
    int worldPosX;
    int worldPosY;
    int currMovementIndex;
    long lastFrameTime;
    int currMovementDuration;
    std::vector<std::pair<int, int> > movementInterpolated;
    std::vector<QuickCG::ColorRGB> ball;

    void createBall();
    void updateBall(std::pair<int, int> from, std::pair<int, int> to);
    void resizeBall(float percentage);
    void linearInterpolation3Points(double x1, double y1, double x2, double y2, 
                                double x3, double y3, double duration);
    void quadraticInterpolation(double x1, double y1, double x2, double y2, double xCurve, double yCurve);
};

#endif