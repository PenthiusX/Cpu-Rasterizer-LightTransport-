#pragma once

#include <QImage>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>

namespace Ras2{
//-----------------------------------------------------------------------------------------------
class Cam {
public:
    Cam();
    ~Cam();

    float viewportHeight = 2.0;
    float viewportWidth;
    float focalLength  = 1.0;

    glm::vec3 origin;
    glm::vec3 horizontal;
    glm::vec3 vertical;

};
//-----------------------------------------------------------------------------------------------
class Light{
public:
    Light(){}
    ~Light(){}

    glm::vec3 pos;
    glm::vec3 dir;

    glm::vec3 col;
};
//-----------------------------------------------------------------------------------------------
class Ray{
public:
    Ray() {}
    Ray(const Ray &r){orig = r.origin(); dir = r.direction()}
    Ray(const glm::vec3 &origin, const glm::vec3 &direction) : orig(origin), dir(direction)
    {}

    glm::vec3 origin() const  { return orig; }
    glm::vec3 direction() const { return dir; }

    glm::vec3 at(float t) const {
        return orig + t * dir;
    }

public:
    glm::vec3 orig;
    glm::vec3 dir;
};
//-----------------------------------------------------------------------------------------------
class Sphere{
public:
    Sphere(){}
    Sphere(glm::vec3 c, float r , glm::vec3 co){
        radius = r;
        center = c;
        color = co;
    }
    ~Sphere(){}

    glm::vec3 center;
    float radius;
    glm::vec3 color;
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
void render()
{
    unsigned int width = 640;
    unsigned int height = 480;

    int n = width * height;
    RGBType *pixels  = new RGBType[n];

    int aspectRatio = width/height;

    double xamnt , yamnt;

    for(unsigned int x = 0 ; x < width ; x++)
    {
        for(unsigned int y = 0 ; y < height; y++)
        {

            unsigned int pos = y*width + x;

            auto r = double(x) / (width-1);
            auto g = double(y) / (height-1);
            auto b = 0.25;

            pixels[pos].r = static_cast<int>(255.999 * r);
            pixels[pos].g = static_cast<int>(255.999 * g);
            pixels[pos].b = static_cast<int>(255.999 * b);

//            if((x > 200 && x < 250)/* && (y > 200 && y < 280)*/){
//                pixels[pos].r = 23;
//                pixels[pos].g = 222;
//                pixels[pos].b = 10;
//            }
//            else{
//                pixels[pos].r = 100;
//                pixels[pos].g = 100;
//                pixels[pos].b = 100;
//            }
        }
    }
    saveBmp(width,height,pixels);
}
//-----------------------------------------------------------------------------------------------
}

