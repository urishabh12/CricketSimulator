#include "ground.h"

namespace Ground {
    void createCricketGround(std::vector<std::vector<Uint32> >& world) {
        // Define the size of the grid (representing the ground)
        int rows = world.size(), cols = world[0].size();

        // Center of the ground
        int centerX = rows / 2, centerY = cols / 2;

        // Draw the outer boundary of the ground (main circle)
        int outerRadius = 70;
        drawCircle(world, centerX, centerY, outerRadius, RGBtoINT(RGB_Green));

        // Draw the inner circle (30-yard circle)
        int innerRadius = 28;
        drawCircle(world, centerX, centerY, innerRadius, RGBtoINT(RGB_Darkgreen));

        // Draw the pitch (rectangle in the center)
        int pitchHeight = 20, pitchWidth = 3;
        drawRectangle(world, centerY - (pitchWidth / 2), centerX - 1, pitchHeight, pitchWidth, RGBtoINT(RGB_Blue));
    }

    // Function to draw a circle on the grid
    void drawCircle(std::vector<std::vector<Uint32> >& grid, int cx, int cy, int r, Uint32 symbol) {
        int rows = grid.size();
        int cols = grid[0].size();
        for (int x = 0; x < rows; ++x) {
            for (int y = 0; y < cols; ++y) {
                // Check if the point lies on the circle boundary
                if (std::round(std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy))) == r) {
                    grid[x][y] = symbol;
                }
            }
        }
    }

    // Function to draw a rectangle on the grid
    void drawRectangle(std::vector<std::vector<Uint32> >& grid, int top, int left, int height, int width, Uint32 symbol) {
        for (int x = top; x < top + height; ++x) {
            for (int y = left; y < left + width; ++y) {
                grid[x][y] = symbol;
            }
        }
    }
}