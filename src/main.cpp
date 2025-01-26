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
  Camera frontCamera("Camera", 1000, 1000, 10);
  World cricketSim("Cricket Sim", 3000, 3000);
  cricketSim.setBlankWorld(RGBtoINT(RGB_White));

  std::vector<std::vector<Uint32> > clippedData;
  screen(frontCamera.getWidth(),frontCamera.getHeight(),0, "Cricket Sim");

  while(!done()) {
    cricketSim.clipScene(clippedData, frontCamera.getHeight(), frontCamera.getWidth());

    int w = frontCamera.getWidth(), h = frontCamera.getHeight();
    //draw the result to the screen
    for(int y = 0; y < h; y++)
    for(int x = 0; x < w; x++)
    {
      pset(x, y, INTtoRGB(clippedData[y][x]));
    }

    redraw();
    cls();
    cricketSim.updateScene();
  }
  return 0;
}

