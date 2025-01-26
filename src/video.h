// video.h
#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <iostream>
#include <cmath>
#include "quickcg.h"

class VideoCG {
public:
    long height;
    long width;
    long fps;
    bool playing;
    long lastFrameTime;
    int lastFrameIndex;
    std::vector<std::vector<QuickCG::ColorRGB> > frames;

    void loadVideo(std::string path, long frameCount, long seconds);
    void play();
    void stop();
    std::vector<QuickCG::ColorRGB>& getNextFrame();
    void paintObject(QuickCG::ColorRGB color);
};

#endif