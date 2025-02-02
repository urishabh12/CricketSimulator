#include "world.h"
#include "ground.h"

//World Positions
int worldHeight = 3000;
int worldWeight = 3000;
std::pair<int, int> onStrikeBatsmanPosition = std::make_pair(-320, 0);
std::pair<int, int> offStrikeBatsmanPosition;
std::pair<int, int> bowlerBowlingPosition = std::make_pair(-150, -50);

// Constructor
World::World(const std::string& name, long height, long width)
    : name(name), height(height), width(width) {
        this->worldData = std::vector<std::vector<Uint32> >(height, std::vector<Uint32>(width, 0));
        this->baseWorldData = this->worldData;
        this->setOnStrikeBatsman();
        this->setBowler();
        this->setBall();
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

void World::setBlankWorld(Uint32 baseColor, Uint32 skipColor) {
    for(int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++) {
            if((i+j) % 16) this->worldData[i][j] = baseColor;
            else this->worldData[i][j] = skipColor;
        }
    }
    this->baseWorldData = this->worldData;
    this->createCricketGround();
}

void World::clipScene(std::vector<std::vector<Uint32> >& clippedData, long h, long w) {
    // Ensure clipping dimensions do not exceed the worldData dimensions
    if (h > height) h = height;
    if (w > width) w = width;

    // Find the pitch
    long centerY = (height / 2) - 300;
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

void World::followBall(std::vector<std::vector<Uint32> >& clippedData, long h, long w) {
    int x = this->ball.worldPosX;
    int y = this->ball.worldPosY;
    if (x == -1 || y == -1) {
        return this->clipScene(clippedData, h, w);
    }

    // Ensure clipping dimensions do not exceed the worldData dimensions
    if (h > height) h = height;
    if (w > width) w = width;

    long centerY = x;
    long centerX = y;

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
    this->updateBowler();
    this->updateBall();
}

void World::updateOnStrikeBatsman() {
    std::vector<QuickCG::ColorRGB> frame = this->onStrikeBatsman.getCurrFrame();
    int h = this->onStrikeBatsman.height, w = this->onStrikeBatsman.width;
    int deltaH = ((this->height - h) / 2) + onStrikeBatsmanPosition.first;
    int deltaW = ((this->width - w) / 2) + onStrikeBatsmanPosition.second;
    this->clearOnWorld(frame, deltaH, deltaW, h, w);

    frame = this->onStrikeBatsman.getNextFrame();
    this->drawOnWorld(frame, deltaH, deltaW, h, w);
}

void World::updateBowler() {
    std::vector<QuickCG::ColorRGB> frame = this->bowler.getCurrFrame();
    int h = this->bowler.height, w = this->bowler.width;
    int deltaH = ((this->height - h) / 2) + bowlerBowlingPosition.first;
    int deltaW = ((this->width - w) / 2) + bowlerBowlingPosition.second;
    this->clearOnWorld(frame, deltaH, deltaW, h, w);

    frame = this->bowler.getNextFrame();
    this->drawOnWorld(frame, deltaH, deltaW, h, w);
}

void World::setOnStrikeBatsman() {
    this->onStrikeBatsman.loadVideo("src/pics/PlayerMovements/DhoniSix", 13, 1);
    this->onStrikeBatsman.resizeVideo(30.0);
    this->onStrikeBatsman.paintObject(QuickCG::ColorRGB(4,118,208));
    this->onStrikeBatsman.play();
}

void World::setBowler() {
    this->bowler.loadVideo("src/pics/PlayerMovements/KulBowling", 5, 1);
    this->bowler.resizeVideo(50.0);
    this->bowler.paintObject(QuickCG::ColorRGB(0,0,139));
    this->bowler.play();
}

void World::setBall() {
    this->ball.createBall();
}

void World::updateBall() {
    int h = this->ball.height, w = this->ball.width;
    int x = this->ball.worldPosX;
    int y = this->ball.worldPosY;
    if (x != -1 || y != -1) {
        this->clearOnWorld(this->ball.ball, x, y, h, w);
    }

    this->ball.updateBall(bowlerBowlingPosition, onStrikeBatsmanPosition);
    h = this->ball.height, w = this->ball.width;
    x = this->ball.worldPosX;
    y = this->ball.worldPosY;
    if (x != -1 || y != -1) {
        this->drawOnWorld(this->ball.ball, x, y, h, w);
    }
}

void World::drawOnWorld(std::vector<QuickCG::ColorRGB> data, int posH, int posW, int h, int w) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (data[i * w + j] == RGB_Black) {
                this->worldData[posH + i][posW + j] = this->worldData[posH + i][posW + j];
                continue;
            }
            this->worldData[posH + i][posW + j] = RGBtoINT(data[i * w + j]);
        }
    }
}

void World::clearOnWorld(std::vector<QuickCG::ColorRGB> data, int posH, int posW, int h, int w) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (RGBtoINT(data[i * w + j]) != this->worldData[posH + i][posW + j]) continue;
            this->worldData[posH + i][posW + j] = this->baseWorldData[posH + i][posW + j];
        }
    }
}

void World::clearWorld() {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            this->worldData[i][j] = this->baseWorldData[i][j];
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
