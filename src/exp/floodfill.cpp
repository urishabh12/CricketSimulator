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


int main(int argc, char *argv[])
{
  unsigned long w = 0, h = 0;
  //declare image buffers
  std::vector<ColorRGB> image1, image2, image3, result;

  //load the images into the buffers. This assumes all have the same size.
  loadImage(image1, w, h, "src/pics/photo1.png");
  loadImage(image2, w, h, "src/pics/photo2.png");
  loadImage(image3, w, h, "src/pics/photo3.png");
  result.resize(w * h);

  //set up the screen
  screen(w,h,0, "Image Arithmetic");

  float img1fr = 1.0;
  float img2fr = 0.0;
  bool direction = true;
  uint64_t prevTime = timeSinceEpochMillisec();
  while(!done()) {
    //Run code at approx 24 frames per second
    uint64_t currTime = timeSinceEpochMillisec();
    if(currTime - prevTime < int(1000/24)) continue;
    prevTime = currTime;
    //do the image arithmetic (here: 'average')
    for(int y = 0; y < h; y++)
    for(int x = 0; x < w; x++)
    {
      result[y * w + x].r = int(image1[y * w + x].r * img1fr + image2[y * w + x].r * img2fr);
      result[y * w + x].g = int(image1[y * w + x].g * img1fr + image2[y * w + x].g * img2fr);
      result[y * w + x].b = int(image1[y * w + x].b * img1fr + image2[y * w + x].b * img2fr);
    }

    //increment and decrement value
    if(direction) {
      img1fr = img1fr - 0.01;
      img2fr = img2fr + 0.01;
    } else {
      img1fr = img1fr + 0.01;
      img2fr = img2fr - 0.01;
    }

    //change direction
    if(img1fr<=0.0 || img2fr<=0.0) {
      direction = !direction;
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
