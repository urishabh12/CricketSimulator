#include "camera.h"

// Constructor
Camera::Camera(const std::string& name, long height, long width, long distance)
    : name(name), height(height), width(width), distance(distance) {}

// Getters
std::string Camera::getName() const {
    return name;
}

long Camera::getHeight() const {
    return height;
}

long Camera::getWidth() const {
    return width;
}

long Camera::getDistance() const {
    return distance;
}

// Setters
void Camera::setName(const std::string& name) {
    this->name = name;
}

void Camera::setHeight(long height) {
    this->height = height;
}

void Camera::setWidth(long width) {
    this->width = width;
}