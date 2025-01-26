#include "world.h"
#include "ground.h"

// Constructor
World::World(const std::string& name, long height, long width)
    : name(name), height(height), width(width) {
        this->worldData = std::vector<std::vector<Uint32> >(height, std::vector<Uint32>(width, 0));
        this->baseWorldData = this->worldData;
        this->setOnStrikeBatsman();
    }

// Getters
std::string World::getName() const {
    return name;
}

long World::getHeight() const {
    return height;
}

long World::getWidth() const {
    return width;
}

std::vector<std::vector<Uint32> > World::getWorldData() {
    return worldData;
}

// Setters
void World::setName(const std::string& name) {
    this->name = name;
}

void World::setBlankWorld(Uint32 color) {
    for(int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++) {
            this->worldData[i][j] = color;
        }
    }
    this->createCricketGround();
}

void World::clipScene(std::vector<std::vector<Uint32> >& clippedData, long h, long w) {
    // Ensure clipping dimensions do not exceed the worldData dimensions
    if (h > height) h = height;
    if (w > width) w = width;

    // Find the center of the worldData
    long centerY = height / 2;
    long centerX = width / 2;

    // Calculate the start and end positions for clipping
    long startY = std::max(0L, centerY - h / 2);
    long startX = std::max(0L, centerX - w / 2);
    long endY = std::min(height, startY + h);
    long endX = std::min(width, startX + w);

    // Resize the clippedData to the required dimensions
    clippedData.resize(h);
    for (auto& row : clippedData) {
        row.resize(w, 0);
    }

    // Copy the relevant portion of worldData into clippedData
    for (long y = startY, cy = 0; y < endY; ++y, ++cy) {
        for (long x = startX, cx = 0; x < endX; ++x, ++cx) {
            clippedData[cy][cx] = worldData[y][x];
        }
    }
}

void World::updateScene() {
    this->updateOnStrikeBatsman();
}

void World::updateOnStrikeBatsman() {
    int lastFrame = this->onStrikeBatsman.lastFrameIndex;
    std::vector<QuickCG::ColorRGB> nextFrame = this->onStrikeBatsman.getNextFrame();
    if (lastFrame == this->onStrikeBatsman.lastFrameIndex) return;
    int h = this->onStrikeBatsman.height, w = this->onStrikeBatsman.width;
    int deltaH = (this->height - h) / 2;
    int deltaW = (this->width - w) / 2;

    this->drawOnWorld(nextFrame, deltaH, deltaW, h, w);
}

void World::setOnStrikeBatsman() {
    this->onStrikeBatsman.loadVideo("src/pics/PlayerMovements/BackfootDefense", 7, 1);
    this->onStrikeBatsman.resizeVideo(50.0);
    this->onStrikeBatsman.paintObject(RGB_White);
    this->onStrikeBatsman.play();
}

void World::drawOnWorld(std::vector<QuickCG::ColorRGB> data, int posH, int posW, int h, int w) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (data[i * w + j] == RGB_Black) {
                this->worldData[posH + i][posW + j] = this->baseWorldData[posH + i][posW + j];
                continue;
            }
            this->worldData[posH + i][posW + j] = RGBtoINT(data[i * w + j]);
        }
    }
}

void World::createCricketGround() {
    unsigned long w = 0, h = 0;
    std::vector<ColorRGB> groundImage;
    loadImage(groundImage, w, h, "src/pics/CricketGroundFrontCropped.png");

    int deltaH = (this->height - h) / 2;
    int deltaW = (this->width - w) / 2;

    this->drawOnWorld(groundImage, deltaH, deltaW, h, w);
    this->baseWorldData = this->worldData;
}
