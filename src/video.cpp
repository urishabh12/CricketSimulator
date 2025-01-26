#include "video.h"

void VideoCG::loadVideo(std::string path, long frameCount, long seconds) {
        this->frames = std::vector<std::vector<QuickCG::ColorRGB> >();
        unsigned long w = 0, h = 0;
        std::vector<QuickCG::ColorRGB> currImage;

        for(int i = 1; i <= frameCount; i++) {
            QuickCG::loadImage(currImage, w, h, path + "/" + std::to_string(i) + ".png");
            this->frames.push_back(currImage);
        }

        this->height = h;
        this->width = w;
        this->playing = false;
        this->lastFrameTime = -1;
        this->fps = ceil(frameCount/seconds);
}

void VideoCG::play() {
    if (this->playing) {
        return;
    }

    this->playing = true;
    this->lastFrameIndex = -1;
}

void VideoCG::stop() {
    this->playing = false;
}

std::vector<QuickCG::ColorRGB>& VideoCG::getNextFrame() {
    if (!this->playing) {
        return this->frames[0];
    }

    if (this->lastFrameTime == -1) {
        this->lastFrameIndex = 0;
        this->lastFrameTime = QuickCG::getTicks();
        return this->frames[this->lastFrameIndex];
    }

    if (this->lastFrameIndex >= this->frames.size() - 1) {
        this->lastFrameIndex = 0;
    }

    int frameChangeInterval = 1000/this->fps;
    long getCurrTime = QuickCG::getTicks();

    if (getCurrTime - this->lastFrameTime > frameChangeInterval) {
        this->lastFrameIndex++;
        this->lastFrameTime = getCurrTime;
    }

    return this->frames[this->lastFrameIndex]; 
}

void VideoCG::paintObject(QuickCG::ColorRGB color) {
    for (int i = 0; i < this->frames.size(); i++) {
        for (int j = 0; j < this->height; j++) {
            for (int k = 0; k < this->width; k++) {
                if (this->frames[i][j * this->width + k] != QuickCG::RGB_Black) {
                    this->frames[i][j * this->width + k] = color;
                }
            }
        }
    }
}
