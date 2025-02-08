#ifndef _quickcg2_h_included
#define _quickcg2_h_included

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm> //std::min and std::max

typedef unsigned char Uint8;
typedef unsigned int Uint32;
typedef unsigned long long Uint64;

namespace QuickCG
{
    //a color with 3 components: r, g and b
    struct ColorRGB
    {
        int r;
        int g;
        int b;

        ColorRGB(Uint8 r, Uint8 g, Uint8 b);
        ColorRGB();
    };

    static const ColorRGB RGB_Black    (  0,   0,   0);
    static const ColorRGB RGB_Red      (255,   0,   0);
    static const ColorRGB RGB_Green    (  0, 255,   0);
    static const ColorRGB RGB_Blue     (  0,   0, 255);
    static const ColorRGB RGB_Cyan     (  0, 255, 255);
    static const ColorRGB RGB_Magenta  (255,   0, 255);
    static const ColorRGB RGB_Yellow   (255, 255,   0);
    static const ColorRGB RGB_White    (255, 255, 255);
    static const ColorRGB RGB_Gray     (128, 128, 128);
    static const ColorRGB RGB_Grey     (192, 192, 192);
    static const ColorRGB RGB_Maroon   (128,   0,   0);
    static const ColorRGB RGB_Darkgreen(  0, 128,   0);
    static const ColorRGB RGB_Navy     (  0,   0, 128);
    static const ColorRGB RGB_Teal     (  0, 128, 128);
    static const ColorRGB RGB_Purple   (128,   0, 128);
    static const ColorRGB RGB_Olive    (128, 128,   0);

    bool operator==(const ColorRGB& color, const ColorRGB& color2);
    bool operator!=(const ColorRGB& color, const ColorRGB& color2);     

    Uint32 RGBtoINT(const ColorRGB& colorRGB);
    ColorRGB INTtoRGB(Uint32 colorINT);

    ////////////////////////////////////////////////////////////////////////////////
    //FILE FUNCTIONS////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    void loadFile(std::vector<unsigned char>& buffer, const std::string& filename);
    void saveFile(const std::vector<unsigned char>& buffer, const std::string& filename);

    ////////////////////////////////////////////////////////////////////////////////
    //IMAGE FUNCTIONS///////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    int loadImage(std::vector<ColorRGB>& out, unsigned long& w, unsigned long& h, const std::string& filename);
    int loadImage(std::vector<Uint32>& out, unsigned long& w, unsigned long& h, const std::string& filename);
    int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);
    int decodePNG(std::vector<unsigned char>& out_image_32bit, unsigned long& image_width, unsigned long& image_height, const std::vector<unsigned char>& in_png);
    long getTicks(); //ticks in milliseconds
    bool done(long startTime, long totalTime);
    uint64_t timeSinceEpochMillisec();
}

#endif