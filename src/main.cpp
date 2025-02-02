#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include "camera.h"
#include "world.h"

#include "quickcg.h"
using namespace QuickCG;

//Zoom: Things on top far on bottom closer

int main(int /*argc*/, char */*argv*/[])
{ 
  Camera frontCamera("Camera", 900, 900, 10);
  World cricketSim("Cricket Sim", 3000, 3000);
  cricketSim.setBlankWorld(RGBtoINT(ColorRGB(49, 47, 54)), RGBtoINT(RGB_White));

  std::vector<std::vector<Uint32> > clippedData;
  screen(frontCamera.getWidth(),frontCamera.getHeight(),0, "Cricket Sim");


  //Timing
  long oldTime, time = getTicks();
  while(!done()) {
    cricketSim.followBall(clippedData, frontCamera.getHeight(), frontCamera.getWidth());

    int w = frontCamera.getWidth(), h = frontCamera.getHeight();
    //draw the result to the screen
    for(int y = 0; y < h; y++)
    for(int x = 0; x < w; x++)
    {
      pset(x, y, INTtoRGB(clippedData[y][x]));
    }

    //timing for input and FPS counter
    oldTime = time;
    time = getTicks();
    double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
    print(1.0 / frameTime); //FPS counter
    redraw();
    cricketSim.updateScene();
  }
  return 0;
}

