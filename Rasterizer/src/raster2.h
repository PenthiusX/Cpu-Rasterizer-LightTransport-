#pragma once

#include <QImage>
#include <time.h>

#include <glm/glm.hpp>

namespace Ras2{
//-----------------------------------------------------------------------------------------------
class Cam {
public:
    Cam(){
        camPos = glm::vec3(0);
        camDir = glm::vec3(0,0,1);
        camRight = glm::vec3(0);
        camDown = glm::vec3(0);
    }
    ~Cam();

    glm::vec3 camPos;
    glm::vec3 camDir;
    glm::vec3 camRight;
    glm::vec3 camDown;
};
//-----------------------------------------------------------------------------------------------
class Ray{
public:
    Ray(){
        origin = glm::vec3(0);
        dir = glm::vec3(1.0,0.0,0.0);
    }
    ~Ray();

    Ray(glm::vec3 o , glm::vec3 d){
        origin = o;
        dir = d;
    }

   glm::vec3 origin;
   glm::vec3 dir;
};
//-----------------------------------------------------------------------------------------------
struct RGBType {
    double r;
    double g;
    double b;
};
//-----------------------------------------------------------------------------------------------
void saveBmp(unsigned int width , unsigned int height ,RGBType *data){
    QImage image(width,height, QImage::Format_RGB32);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
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
//-----------------------------------------------------------------------------------------------
void render(){
    unsigned int width =400;
    unsigned int height = 400;

//    int dpi = 72;
    int n = width * height;
    RGBType *pixels  = new RGBType[n];

    for(unsigned int x = 0 ; x < width ; x++){
        for(unsigned int y = 0 ; y < height; y++){

           unsigned int pos = y*width + x;

           if((x > 200 && x < 250)/* && (y > 200 && y < 280)*/){
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
//-----------------------------------------------------------------------------------------------
}

