#include <opencv2/opencv.hpp>
#include <iostream>

// Function to simulate retrieving a frame's pixel data.
// In practice, replace this with your code that fills in the RGB data.
void getFrameData(unsigned char* data, int width, int height, int frameIndex) {
    // Example: fill the frame with a gradient that changes over time.
    // Data is expected in row-major order, with 3 bytes per pixel (RGB).
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 3;
            data[idx]     = static_cast<unsigned char>((x + frameIndex) % 256); // Red
            data[idx + 1] = static_cast<unsigned char>((y + frameIndex) % 256); // Green
            data[idx + 2] = static_cast<unsigned char>(frameIndex % 256);         // Blue
        }
    }
}

int main() {
    // Video parameters
    std::cout << "hello world" << std::endl;
    int width = 640;
    int height = 480;
    int fps = 30;
    int numFrames = 100; // Total number of frames to write

    // Define the codec and create VideoWriter object.
    // You can try other codecs, e.g., 'XVID', 'MJPG', etc.
    cv::VideoWriter writer("output.avi", 
                           cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 
                           fps, cv::Size(width, height));

    if (!writer.isOpened()) {
        std::cerr << "Could not open the output video file for write.\n";
        return -1;
    }

    // Allocate a buffer for a single frame's pixel data.
    // Each pixel has 3 bytes (RGB).
    std::vector<unsigned char> frameBuffer(width * height * 3);

    for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {
        // Fill frameBuffer with the RGB data for the current frame.
        getFrameData(frameBuffer.data(), width, height, frameIndex);

        // Create a cv::Mat from the frameBuffer.
        // Note: If your data is in RGB order and you want to display/save it as is,
        // you might need to convert it to BGR since OpenCV uses BGR by default.
        cv::Mat frame(height, width, CV_8UC3, frameBuffer.data());

        // Optionally, convert RGB to BGR if needed:
        cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);

        // Write the frame to the video file.
        writer.write(frame);
    }

    // Release the video writer (also flushes any buffered data).
    writer.release();

    std::cout << "Video successfully saved as output.avi\n";
    return 0;
}
