/*
Copyright (c) 2004, Lode Vandevenne
All rights reserved.
*/

#include <cmath>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>

#include "quickcg.h"
using namespace QuickCG;
using namespace std;

//place the example code below here:

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

std::unordered_map<Uint32, bool> removeablePixel;

std::vector<ColorRGB> getChangeMatrix(std::vector<ColorRGB>& input, int h, int w) {
  std::vector<ColorRGB> result(w * h);
  std::vector<Uint32> intRepOfInput(w * h);
  for(int y = 0; y < h; y++) 
  {
    for(int x = 0; x < w; x++)
    {
      Uint32 c = RGBtoINT(input[y * w + x]);
      intRepOfInput[y * w + x] = c;
    }
  }

  for(int y = 0; y < h; y++) 
  {
    for(int x = 0; x < w; x++)
    {
      if(removeablePixel[intRepOfInput[y * w + x]]) {
        result[y * w + x] = RGB_White;
      } else {
        result[y * w + x] = input[y * w + x];
      }
    }
  }
  return result;
}

void preparePixelsToBeRemoved(std::vector<ColorRGB>& a, std::vector<ColorRGB>& b, int h, int w) {
  for(int y = 0; y < h; y++) 
  {
    for(int x = 0; x < w; x++)
    {
        if(a[y * w + x] == RGB_White) 
        {
          removeablePixel[RGBtoINT(b[y * w + x])] = true;
        }
    }
  }
}


int main(int argc, char *argv[])
{
  unsigned long w = 0, h = 0;
  //declare image buffers
  std::vector<ColorRGB> image1, image2, image3, result;

  //load the images into the buffers. This assumes all have the same size.
  loadImage(image1, w, h, "src/pics/defense/1.png");
  result.resize(w * h);

  preparePixelsToBeRemoved(image1, image2, h, w);

  //set up the screen
  screen(w,h,0, "Image Arithmetic");

  int currImage = 1;
  uint64_t prevTime = timeSinceEpochMillisec();
  while(!done()) {
    std::string path = "src/pics/defense/" + std::to_string(currImage) + ".png";
    loadImage(image1, w, h, path);
    currImage++;
    if (currImage > 75) {
      currImage = 1;
    }
    //Run code at approx 24 frames per second
    uint64_t currTime = timeSinceEpochMillisec();
    if(currTime - prevTime < int(1000/24)) continue;
    prevTime = currTime;
    //do the image arithmetic (here: 'average')
    for(int y = 0; y < h; y++)
    for(int x = 0; x < w; x++)
    {
      result[y * w + x].r = image1[y * w + x].r;
      result[y * w + x].g = image1[y * w + x].g;
      result[y * w + x].b = image1[y * w + x].b;
    }

    //draw the result buffer to the screen
    for(int y = 0; y < h; y++)
    for(int x = 0; x < w; x++)
    {
      pset(x, y, result[y * w + x]);
    }

    //redraw & sleep
    redraw();
    cls();
  }

  return 0;
}
