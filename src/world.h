// world.h
#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <iostream>
#include "quickcg2.h"
#include "ball.h"
#include "video.h"

class World {
private:
    std::string name;
    long height;
    long width;
    std::vector<std::vector<Uint32> > worldData;
    std::vector<std::vector<Uint32> > baseWorldData;
    VideoCG onStrikeBatsman;
    VideoCG bowler;
    Ball ball;

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
    void setBlankWorld(Uint32 baseColor, Uint32 skipColor);

    void clipScene(std::vector<std::vector<Uint32> >& clippedData, long h, long w);
    void followBall(std::vector<std::vector<Uint32> >& clippedData, long h, long w);
    void updateScene();
    void updateOnStrikeBatsman();
    void setOnStrikeBatsman();
    void updateBowler();
    void setBowler();
    void updateBall();
    void setBall();
    void drawOnWorld(std::vector<QuickCG::ColorRGB> data, int posX, int posY, int h, int w);
    void clearOnWorld(std::vector<QuickCG::ColorRGB> data, int posX, int posY, int h, int w);
    void clearWorld();
};

#endif // WORLD_H
