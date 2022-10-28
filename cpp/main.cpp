#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdint.h>

const uint32_t boxSize = 1000;
const uint32_t maxIterations = 100;
const double infinityValue = 20.0;

void drawMandelBrot(uint8_t* data, double mapping);
double map(double value, double iStart, double iStop, double oStart, double oStop);
void setPixel(uint8_t* data, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void HSVtoRGB(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b);

int main() {
    uint8_t* data = new uint8_t[boxSize * boxSize * 3];

    drawMandelBrot(data, 0.03);

    stbi_write_jpg("out.jpg", boxSize, boxSize, 3, data, 100);
    return 0;
}

void drawMandelBrot(uint8_t* data, double mapping) {
    for(uint32_t y = 0; y < boxSize; y++){
        for(uint32_t x = 0; x < boxSize; x++){
            double a = map((double)x, 0.0, boxSize, -mapping, mapping) -0.16;
            double b = map((double)y, 0.0, boxSize, -mapping, mapping) +1.0405;

            double cA = a;
            double cB = b;

            uint32_t n;
            for(n = 0; n < maxIterations; n++) {
                double re = a * a - b * b;
                double im = 2.0 * a * b;
                a = re + cA;
                b = im + cB;

                if((a * a + b * b) > infinityValue) {
                    break;
                }
            }

            if (n < maxIterations) {
                int i = n % 16;
                uint8_t mapping[] = {
                    66, 30, 15,
                    25, 7, 26,
                    9, 1, 47,
                    4, 4, 73,
                    0, 7, 100,
                    12, 44, 138,
                    24, 82, 177,
                    57, 125, 209,
                    134, 181, 229,
                    211, 236, 248,
                    241, 233, 191,
                    248, 201, 95,
                    255, 170, 0,
                    204, 128, 0,
                    153, 87, 0,
                    106, 52, 3
                };
                setPixel(data, x, y, mapping[i*3], mapping[i*3+1], mapping[i*3+2]);
            }else{
                setPixel(data, x, y, 0, 0, 0);
            }
        }
    }
}

double map(double value, double iStart, double iStop, double oStart, double oStop) {
    return oStart + (oStop - oStart) * ((value - iStart) / (iStop - iStart));
}

void setPixel(uint8_t* data, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    data[y * boxSize * 3 + x * 3] = r;
    data[y * boxSize * 3 + x * 3 + 1] = g;
    data[y * boxSize * 3 + x * 3 + 2] = b;
}

void HSVtoRGB(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b) {
    float i, f, p, q, t;
    i = floor(h * 6.0);
    f = h * 6.0 - i;
    p = v * (1.0 - s);
    q = v * (1.0 - f * s);
    t = v * (1.0 - (1.0 - f) * s);

    int index = ((int)i) % 6;

    if(index == 0){
        r = v * 255.0f;
        g = t * 255.0f;
        b = p * 255.0f;
    }else if(index == 1){
        r = q * 255.0f;
        g = v * 255.0f;
        b = p * 255.0f;
    }else if(index == 2){
        r = p * 255.0f;
        g = v * 255.0f;
        b = t * 255.0f;
    }else if(index == 3){
        r = p * 255.0f;
        g = q * 255.0f;
        b = v * 255.0f;
    }else if(index == 4){
        r = t * 255.0f;
        g = p * 255.0f;
        b = v * 255.0f;
    }else if(index == 5){
        r = v * 255.0f;
        g = p * 255.0f;
        b = q * 255.0f;
    }
}