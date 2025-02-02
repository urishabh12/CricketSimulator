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
        // this->lastFrameIndex = 0;
        return this->frames[this->lastFrameIndex];
    }

    int frameChangeInterval = 1000/this->fps;
    long getCurrTime = QuickCG::getTicks();
    long timeDiff = getCurrTime - this->lastFrameTime;

    if (timeDiff > frameChangeInterval) {
        this->lastFrameIndex += ceil(timeDiff / frameChangeInterval);
        this->lastFrameTime = getCurrTime;
    }

    return this->frames[this->lastFrameIndex]; 
}

std::vector<QuickCG::ColorRGB>& VideoCG::getCurrFrame() {
    if (!this->playing || this->lastFrameIndex == -1) {
        return this->frames[0];
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

void VideoCG::resizeVideo(float percentage) {
    if (percentage <= 0 || percentage > 100) {
        throw std::invalid_argument("Percentage must be between 0 and 100.");
    }

    // Get the original dimensions
    int originalHeight = this->height;
    int originalWidth = this->width;

    // Calculate new dimensions while keeping aspect ratio
    int newHeight = static_cast<int>(originalHeight * (percentage / 100.0));
    int newWidth = static_cast<int>(originalWidth * (percentage / 100.0));

    // Scale factors
    float rowScale = static_cast<float>(originalHeight) / newHeight;
    float colScale = static_cast<float>(originalWidth) / newWidth;

    // Fill the resized image using nearest-neighbor interpolation
    for (int k = 0; k < this->frames.size(); k++) {
        std::vector<QuickCG::ColorRGB> resizedImage(newHeight * newWidth);
        for (int i = 0; i < newHeight; ++i) {
            for (int j = 0; j < newWidth; ++j) {
                int origRow = static_cast<int>(i * rowScale);
                int origCol = static_cast<int>(j * colScale);
                resizedImage[i * newWidth + j] = this->frames[k][origRow * originalWidth + origCol];
            }
        }
        this->frames[k] = resizedImage;
    }

    this->height = newHeight;
    this->width = newWidth;

    return;
}
