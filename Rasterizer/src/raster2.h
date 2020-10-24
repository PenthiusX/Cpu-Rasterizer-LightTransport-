#pragma once

#include <QImage>
#include <time.h>

namespace Ras2{
struct RGBType {
    double r;
    double g;
    double b;
};

void saveBmp(unsigned int width , unsigned int height ,RGBType *data){
    QImage image(400,400, QImage::Format_RGB32);

    for (int x = 0; x < 400; x++) {
        for (int y = 0; y < 400; y++) {
            unsigned int pos = y * width + x;
            RGBType rgb = data[pos];

            double red = (data[pos].r)*255;
            double green = (data[pos].g)*255;
            double blue = (data[pos].b)*255;

            unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};

            image.setPixel(x, y, qRgb(min((int)color[0], 255),min((int)color[1], 255),min((int)color[2], 255)));
        }
    }
    image.save("Finalsave.jpeg", 0, -1);
}

void render(){
    unsigned int width =400;
    unsigned int height = 400;

//    int dpi = 72;
    int n = width * height;
    RGBType *pixels  = new RGBType[n];

    for(unsigned int x = 0 ; x < width ; x++){
        for(unsigned int y = 0 ; y < height; y++){

           unsigned int pos = y * width + x;

           if((x > 200 && x < 300)/* && (y > 200 && y < 280)*/){
               pixels[pos].r = 23;
               pixels[pos].g = 222;
               pixels[pos].b = 10;
           }
           else{
               pixels[pos].r = 100;
               pixels[pos].g = 100;
               pixels[pos].b = 100;
           }

        }
    }
    saveBmp(width,height,pixels);
}

}

