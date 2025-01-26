// world.h
#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <iostream>
#include "quickcg.h"
#include "video.h"

class World {
private:
    std::string name;
    long height;
    long width;
    std::vector<std::vector<Uint32> > worldData;
    std::vector<std::vector<Uint32> > baseWorldData;
    VideoCG onStrikeBatsman;

    void createCricketGround();

public:
    // Constructor
    World(const std::string& name, long height, long width);

    // Getters
    std::string getName() const;
    long getHeight() const;
    long getWidth() const;
    std::vector<std::vector<Uint32> > getWorldData();

    // Setters
    void setName(const std::string& brand);
    void setBlankWorld(Uint32 color);

    void clipScene(std::vector<std::vector<Uint32> >& clippedData, long h, long w);
    void updateScene();
    void updateOnStrikeBatsman();
    void setOnStrikeBatsman();
    void drawOnWorld(std::vector<QuickCG::ColorRGB> data, int posX, int posY, int h, int w);
};

#endif // WORLD_H
