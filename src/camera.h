// Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <iostream>

class Camera {
private:
    std::string name;
    long height;
    long width;
    long distance;

public:
    // Constructor
    Camera(const std::string& name, long height, long width, long distance);

    // Getters
    std::string getName() const;
    long getHeight() const;
    long getWidth() const;
    long getDistance() const;

    // Setters
    void setName(const std::string& brand);
    void setHeight(long resolution);
    void setWidth(long focalLength);
};

#endif // CAMERA_H
