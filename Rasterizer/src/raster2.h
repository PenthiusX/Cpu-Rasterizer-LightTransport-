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
    Ray(const Ray &r){orig = r.origin(); dir = r.direction();}
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

float clip(int n, int lower, int upper) {
  return std::max(lower, std::min(n, upper));
}
void saveBmp(unsigned int width , unsigned int height ,RGBType *data){
    QImage image(width,height, QImage::Format_RGB32);

    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            unsigned int pos = y * height + x;
            RGBType rgb = data[pos];

            double red = (data[pos].r)*255;
            double green = (data[pos].g)*255;
            double blue = (data[pos].b)*255;

            unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};

            image.setPixel(x, y, qRgb(clip((int)color[0],0,255),clip((int)color[1],0, 255),clip((int)color[2],0, 255)));
        }
    }
    image.save("Finalsave.jpeg", 0, -1);
}

//-----------------------------------------------------------------------------------------------
void render()
{
    unsigned int width = 50;
    unsigned int height = 50;

//    int n = width * height;
//    RGBType *pixels  = new RGBType[n];

    int aspectRatio = width/height;

     QImage image(width,height, QImage::Format_RGB32);

//    Ray r(glm::vec3(0),glm::vec3(0));
//    Ray r1 = r;

    for(unsigned int x = height; x > 0 ; x--)//starts from max heights // topLeft edge // and moves to 0 bottom left
    {
        for(unsigned int y = 0 ; y < width; ++y)
        {
//            unsigned int pos = y * height + x;

            auto r = double(x) / (height-1);
            auto g = double(y) / (width-1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            image.setPixel(x, y, qRgb(clip((int)ir,0,255),clip((int)ig,0, 255),clip((int)ib,0, 255)));
        }
    }
//    saveBmp(width,height,pixels);
    image.save("Finalsave.jpeg", 0, -1);
}
//-----------------------------------------------------------------------------------------------
}

