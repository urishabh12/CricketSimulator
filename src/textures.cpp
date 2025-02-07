#include "textures.h"

using Uint8 = unsigned char;

namespace Textures {
int p[512];

// Initializes the permutation table (using Ken Perlin's original permutation).
void initPermutation() {
    int permutation[256] = {
        151,160,137,91,90,15,
        131,13,201,95,96,53,194,233,7,225,140,36,
        103,30,69,142,8,99,37,240,21,10,23,
        190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
        35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,
        168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,
        111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
        102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,
        89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,
        186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,
        82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,
        183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,
        43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,
        185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,
        179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,
        199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,138,
        236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,
        61,156,180
    };

    for (int i = 0; i < 256; i++) {
        p[i] = permutation[i];
        p[256 + i] = permutation[i];
    }
}

// Fade function as defined by Ken Perlin.
double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// Linear interpolation.
double lerp(double t, double a, double b) {
    return a + t * (b - a);
}

// Gradient function calculates dot product between a pseudorandom
// gradient vector and the vector from the input coordinate to the grid corner.
double grad(int hash, double x, double y, double z) {
    int h = hash & 15;           // Take the lower 4 bits of the hash.
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

// 3D Perlin noise function.
double perlin(double x, double y, double z) {
    int X = static_cast<int>(floor(x)) & 255;
    int Y = static_cast<int>(floor(y)) & 255;
    int Z = static_cast<int>(floor(z)) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    int A  = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B  = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;

    double res = lerp(w,
                  lerp(v,
                       lerp(u, grad(p[AA],   x,   y,   z),
                                grad(p[BA],   x - 1, y,   z)),
                       lerp(u, grad(p[AB],   x,   y - 1, z),
                                grad(p[BB],   x - 1, y - 1, z))),
                  lerp(v,
                       lerp(u, grad(p[AA + 1], x,   y,   z - 1),
                                grad(p[BA + 1], x - 1, y,   z - 1)),
                       lerp(u, grad(p[AB + 1], x,   y - 1, z - 1),
                                grad(p[BB + 1], x - 1, y - 1, z - 1)))
                );
    return (res + 1.0) / 2.0; // Normalize result to [0, 1]
}

std::vector<QuickCG::ColorRGB> getCloudTexture(int h, int w) {
    const int width = w;
    const int height = h;

    // Create an array (vector) of ColorRGB for the image.
    std::vector<QuickCG::ColorRGB> image(width * height);

    // Initialize the permutation table for Perlin noise.
    initPermutation();

    // Parameters for the smoke texture.
    double scale = 0.02;  // Frequency scale for the noise.
    int octaves = 5;      // Number of noise octaves.

    // Generate the smoke texture.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double amplitude = 1.0;
            double frequency = scale;
            double noiseSum = 0.0;
            double totalAmplitude = 0.0;

            // Sum several octaves to create a turbulent effect.
            for (int i = 0; i < octaves; ++i) {
                double noiseValue = perlin(x * frequency, y * frequency, 0.0);
                noiseSum += noiseValue * amplitude;
                totalAmplitude += amplitude;
                frequency *= 2.0;
                amplitude *= 0.5;
            }

            // Normalize the noise value to [0, 1] and optionally enhance contrast.
            double normalized = noiseSum / totalAmplitude;
            normalized = pow(normalized, 1.5);

            // Convert the normalized value to an intensity between 0 and 255.
            int intensity = static_cast<int>(normalized * 255);
            intensity = std::min(255, std::max(0, intensity));

            // Set the RGB values (using the same intensity for a grayscale look).
            QuickCG::ColorRGB pixel(static_cast<Uint8>(intensity), static_cast<Uint8>(intensity), static_cast<Uint8>(intensity));
            image[y * width + x] = pixel;
        }
    }

    return image;
}

std::vector<QuickCG::ColorRGB> getWaterTexture(int h, int w) {
    // Image dimensions.
    const int width = w;
    const int height = h;

    // Initialize the permutation table for noise.
    initPermutation();

    // Container for pixel data (RGB image).
    // Each pixel has three components (R, G, B).
    std::vector<QuickCG::ColorRGB> image(width * height);

    // Parameters for the water texture.
    double waveFrequency = 0.08;         // Base frequency for the sine waves.
    double noiseScale = 0.05;            // Scale factor for noise distortion.
    double distortionAmplitude = 15.0;   // How much the noise distorts the wave pattern.

    // Generate the water texture.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Use Perlin noise to distort the coordinates,
            // which gives the waves a more natural, irregular appearance.
            double distortionX = perlin(x * noiseScale, y * noiseScale, 0.0) * distortionAmplitude;
            double distortionY = perlin(x * noiseScale, y * noiseScale, 1.0) * distortionAmplitude;

            // Create a periodic wave pattern with sine functions.
            double waveX = sin((x + distortionX) * waveFrequency);
            double waveY = sin((y + distortionY) * waveFrequency);
            
            // Combine the two wave components.
            // The sum lies in the range [-2, 2]. Normalize it to [0, 1].
            double waveValue = (waveX + waveY);
            double normalized = (waveValue + 2.0) / 4.0;

            // Map the normalized value to a blue-based color gradient.
            // For example, low values give a deep blue and high values give a lighter blue.
            // You can customize these base colors as desired.
            unsigned char red   = 0;  // Keeping red at 0 for a blue tint.
            unsigned char green = static_cast<unsigned char>(normalized * 150);  // 0 to 150.
            unsigned char blue  = static_cast<unsigned char>(80 + normalized * (255 - 80));  // 80 to 255.

            // Compute the index for the current pixel (each pixel has 3 values).
            int index = (y * width + x);
            image[index] = QuickCG::ColorRGB(red, green, blue) ;
        }
    }

    return image;
}

std::vector<QuickCG::ColorRGB> getGrassTexture(int h, int w) {
    const int width = w;
    const int height = h;
    std::vector<QuickCG::ColorRGB> image(width * height);

    initPermutation();

    // Parameters for multi-octave noise.
    const int octaves = 5;
    const double persistence = 0.5;
    const double baseFrequency = 0.05;

    // Loop over each pixel.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double noiseValue = 0.0;
            double amplitude = 1.0;
            double maxAmplitude = 0.0;
            double frequency = baseFrequency;
            
            // Sum multiple octaves of Perlin noise.
            for (int i = 0; i < octaves; i++) {
                noiseValue += perlin(x * frequency, y * frequency, 0.0) * amplitude;
                maxAmplitude += amplitude;
                amplitude *= persistence;
                frequency *= 2.0;
            }
            noiseValue /= maxAmplitude; // Normalize to [0,1]

            // Map the noise value to a green grass color.
            // Base color values are chosen to be in a green range.
            // Here, red and blue vary slightly, while green varies over a wider range.
            unsigned char r = static_cast<unsigned char>(30 + noiseValue * 40);  // 30 to 70
            unsigned char g = static_cast<unsigned char>(150 + noiseValue * 105); // 150 to 255
            unsigned char b = static_cast<unsigned char>(30 + noiseValue * 40);  // 30 to 70

            int index = (y * width + x);
            image[index] = QuickCG::ColorRGB(r, g, b);
        }
    }

    return image;
}

std::vector<QuickCG::ColorRGB> getBrownGroundTexture(int h, int w) {
    const int width = w;
    const int height = h;
    std::vector<QuickCG::ColorRGB> image(width * height);

    initPermutation();

    // Parameters for multi-octave noise.
    const int octaves = 5;
    const double persistence = 0.5;
    const double baseFrequency = 0.05;

    // Loop over each pixel.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double noiseValue = 0.0;
            double amplitude = 1.0;
            double maxAmplitude = 0.0;
            double frequency = baseFrequency;
            
            // Sum multiple octaves of Perlin noise.
            for (int i = 0; i < octaves; i++) {
                noiseValue += perlin(x * frequency, y * frequency, 0.0) * amplitude;
                maxAmplitude += amplitude;
                amplitude *= persistence;
                frequency *= 2.0;
            }
            noiseValue /= maxAmplitude; // Normalize to [0,1]

            // Map the noise value to a brown ground color.
            // Adjust the ranges below to tweak the color.
            // For example, a typical brown might have a moderate red, lower green, and low blue.
            unsigned char r = static_cast<unsigned char>(80 + noiseValue * 50);  // Range: 80 to 130
            unsigned char g = static_cast<unsigned char>(40 + noiseValue * 30);  // Range: 40 to 70
            unsigned char b = static_cast<unsigned char>(20 + noiseValue * 20);  // Range: 20 to 40

            int index = (y * width + x);
            image[index] = QuickCG::ColorRGB(r, g, b);
        }
    }

    return image;
}

}