#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <sys/stat.h>
#include "stb_image_write.h"
#include <stdint.h>

const uint32_t numFrames = 100;
const uint32_t boxSize = 100;
const uint32_t maxIterations = 100;
const double infinityValue = 20.0;

void drawMandelBrot(uint8_t* data, double mapping);
double map(double value, double iStart, double iStop, double oStart, double oStop);
void setPixel(uint8_t* data, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void HSVtoRGB(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b);

int main() {
    uint8_t* data = new uint8_t[boxSize * boxSize * 3];

	mkdir("frames", 0777);

	for(uint32_t i = 0; i < numFrames; i++){
    	drawMandelBrot(data, 2.0 / pow(2.0, i / 10.0));
    	stbi_write_jpg((std::string("frames/") + std::to_string(i) + ".jpg").c_str(), boxSize, boxSize, 3, data, 100);
		std::cout << "hi" << std::endl;
	}

    return 0;
}

void drawMandelBrot(uint8_t* data, double mapping) {
    for(uint32_t x = 0; x < boxSize; x++){
        for(uint32_t y = 0; y < boxSize; y++){
            double a = map((double)x, 0.0, boxSize, -mapping, mapping) - 0.7453;
            double b = map((double)y, 0.0, boxSize, -mapping, mapping) - 0.1127;

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

            float hue = n / (float)maxIterations;
            float saturation = 1.0f;
            float value;
            if(n < maxIterations){
                value = 1.0f;
            }else{
                value = 0.0f;
            }

            uint8_t red, green, blue;
            HSVtoRGB(hue, saturation, value, red, green, blue);

            setPixel(data, x, y, green, red, blue);
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