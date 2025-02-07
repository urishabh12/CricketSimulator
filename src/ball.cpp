#include "ball.h"

void Ball::createBall() {
    this->height = 5;
    this->width = 5;
    int centerX = this->height / 2;
    int centerY = this->width / 2;
    int radius = this->height / 2;
    this->worldPosX = -1;
    this->worldPosY = -1;
    this->currMovementIndex = -1;
    this->ball = std::vector<QuickCG::ColorRGB>(this->height * this->width);

    // Loop through each cell in the array
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (std::pow(i - centerX, 2) + std::pow(j - centerY, 2) <= std::pow(radius, 2)) {
                this->ball[i * this->width + j] = QuickCG::RGB_Red; 
            } else {
                this->ball[i * this->width + j] = QuickCG::RGB_Black;
            }
        }
    }
}

void Ball::updateBall(std::pair<int, int> bowlerPosition, std::pair<int, int> batsmanPosition) {
    if (this->currMovementIndex == -1) {
        //Bowler to batsman
        int deltaX = (3000 - this->height) / 2;
        int deltaY = (3000 - this->width) / 2;
        //cover drive
        batsmanPosition.second -= 25;
        int x1 = deltaX + bowlerPosition.first, y1 = deltaY + bowlerPosition.second, 
                    x3 = deltaX + batsmanPosition.first, y3 = deltaY + batsmanPosition.second;
        int x2 = (x3 + x1) / 2;
        int y2 = (y3 + y1) / 2;
        this->linearInterpolation3Points(x1, y1, x2, y2, x3, y3, 1);
        this->currMovementIndex = 0;
        this->lastFrameTime = QuickCG::getTicks();
        this->currMovementDuration = 1000;

        //Batsman to boundary
        int x4 = deltaX + 800, y4 = deltaY - 800;
        this->quadraticInterpolation(x3, y3, x4, y4, -1800, -500);
    }

    if (this->currMovementIndex >= this->movementInterpolated.size()) {
        this->currMovementDuration = 720;
        this->currMovementIndex = 0;
    }

    if (this->currMovementIndex >= this->movementInterpolated.size() / 2) {
        this->currMovementDuration = 2500;
    }

    int frameChangeInterval = this->currMovementDuration/24;
    long getCurrTime = QuickCG::getTicks();
    long timeDiff = getCurrTime - this->lastFrameTime;
    //std::cout << frameChangeInterval << ' ' << timeDiff << std::endl;

    if (timeDiff > frameChangeInterval) {
        this->currMovementIndex += ceil(timeDiff / frameChangeInterval);
        this->lastFrameTime = getCurrTime;
    }


    this->worldPosX = this->movementInterpolated[this->currMovementIndex].first;
    this->worldPosY = this->movementInterpolated[this->currMovementIndex].second;
}

void Ball::quadraticInterpolation(double x1, double y1, double x2, double y2, double xCurve, double yCurve) {
    double xm = ((x1 + x2) / 2) + xCurve;
    double ym = ((y1 + y2) / 2) + yCurve;

    for (double t = 0.081; t <= 1; t += 0.041) {
        double xt = (1 - t) * (1 - t) * x1 + 2 * (1 - t) * t * xm + t * t * x2;
        double yt = (1 - t) * (1 - t) * y1 + 2 * (1 - t) * t * ym + t * t * y2;

        this->movementInterpolated.push_back(std::make_pair(xt, yt));
    }
}

void Ball::linearInterpolation3Points(double x1, double y1, double x2, double y2, 
                                double x3, double y3, double duration) {
    int steps = 12; // Number of steps per segment
    double t_step = 1.0 / steps;
    std::cout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << ' ' << x3 << ' ' << y3 << std::endl;

    // Interpolate between (x1, y1) and (x2, y2)
    for (double t = 0; t <= 1; t += t_step) {
        double xt = x1 + t * (x2 - x1);
        double yt = y1 + t * (y2 - y1);
        this->movementInterpolated.push_back(std::make_pair(xt, yt));
    }

    // Interpolate between (x2, y2) and (x3, y3)
    for (double t = 0; t <= 1; t += t_step) {
        double xt = x2 + t * (x3 - x2);
        double yt = y2 + t * (y3 - y2);
        this->movementInterpolated.push_back(std::make_pair(xt, yt));
    }
}

void Ball::resizeBall(float percentage) {
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


    std::vector<QuickCG::ColorRGB> resizedImage(newHeight * newWidth);
    for (int i = 0; i < newHeight; ++i) {
        for (int j = 0; j < newWidth; ++j) {
            int origRow = static_cast<int>(i * rowScale);
            int origCol = static_cast<int>(j * colScale);
            resizedImage[i * newWidth + j] = this->ball[origRow * originalWidth + origCol];
        }
    }
    this->ball = resizedImage;

    this->height = newHeight;
    this->width = newWidth;

    return;
}
