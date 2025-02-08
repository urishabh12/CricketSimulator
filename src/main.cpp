#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include "camera.h"
#include "world.h"
#include <opencv2/opencv.hpp>

#include "quickcg2.h"
using namespace QuickCG;

void getFrameData(unsigned char* data, int width, int height, std::vector<std::vector<Uint32> > frameData) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 3;
            ColorRGB colorData = INTtoRGB(frameData[y][x]);
            data[idx]     = colorData.r; // Red
            data[idx + 1] = colorData.g; // Green
            data[idx + 2] = colorData.b; // Blue
        }
    }
}


int main(int /*argc*/, char */*argv*/[])
{ 
  // Video parameters
  int width = 900;
  int height = 900;
  int fps = 24;
  Camera frontCamera("Camera", height, width, 10);
  World cricketSim("Cricket Sim", 3000, 3000);
  cricketSim.setBlankWorld(RGBtoINT(ColorRGB(49, 47, 54)), RGBtoINT(RGB_White));

  std::vector<std::vector<Uint32> > clippedData;

  // Define the codec and create VideoWriter object.
  // Here we use "mp4v" for the MP4 container.
  cv::VideoWriter writer("output.mp4", 
                           cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 
                           fps, cv::Size(width, height));

  if (!writer.isOpened()) {
    std::cerr << "Could not open the output video file for write.\n";
    return -1;
  }

  std::vector<unsigned char> frameBuffer(width * height * 3);


  //Timing
  long oldTime, time = getTicks();
  long loopStartTime = getTicks();
  while(!done(loopStartTime, 10000)) {
    cricketSim.followBall(clippedData, frontCamera.getHeight(), frontCamera.getWidth());
    getFrameData(frameBuffer.data(), width, height, clippedData);
    cv::Mat frame(height, width, CV_8UC3, frameBuffer.data());
    cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
    writer.write(frame);

    //timing for input and FPS counter
    oldTime = time;
    time = getTicks();
    double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
    std::cout << 1.0 / frameTime << std::endl;
    cricketSim.updateScene();
  }

  // Release the video writer.
  writer.release();
  return 0;
}

