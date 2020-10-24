#pragma once
#include <time.h>

struct RGBType {
    double r;
    double g;
    double b;
};

void saveBmp(){

}

void render(unsigned int width , unsigned int height ){

    int dpi = 72;
    int n = width * height;
    RGBType *pixels  = new RGBType[n];

    for(unsigned int x = 0 ; x < width ; x++){
        for(unsigned int y = 0 ; y < height; y++){

        }
    }
}
