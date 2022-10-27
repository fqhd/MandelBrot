#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdint.h>

const uint32_t boxSize = 1000;
const uint32_t maxIterations = 300;
const double infinityValue = 50.0;

void drawMandelBrot(uint8_t* data, double mapping);
double map(double value, double iStart, double iStop, double oStart, double oStop);
void setPixel(uint8_t* data, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void HSVtoRGB(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b);

int main() {
    uint8_t* data = new uint8_t[boxSize * boxSize * 3];

    drawMandelBrot(data, 2.0);

    stbi_write_jpg("out.jpg", boxSize, boxSize, 3, data, 100);
    return 0;
}

void drawMandelBrot(uint8_t* data, double mapping) {
    for(uint32_t x = 0; x < boxSize; x++){
        for(uint32_t y = 0; y < boxSize; y++){
            double a = map((double)x, 0.0, boxSize, -mapping, mapping);
            double b = map((double)y, 0.0, boxSize, -mapping, mapping);

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

            float hue = int(255.0f * n / (float)maxIterations);
            float saturation = 255.0f;
            float value;
            if(n < maxIterations){
                value = 255.0f;
            }else{
                value = 0.0f;
            }

            uint8_t red, green, blue;
            HSVtoRGB(hue, saturation, value, red, green, blue);

            setPixel(data, x, y, red, green, blue);
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
        r = v;
        g = t;
        b = p;
    }else if(index == 1){
        r = q;
        g = v;
        b = p;
    }else if(index == 2){
        r = p;
        g = v;
        b = t;
    }else if(index == 3){
        r = p;
        g = q;
        b = v;
    }else if(index == 4){
        r = t;
        g = p;
        b = v;
    }else if(index == 5){
        r = v;
        g = p;
        b = q;
    }
}