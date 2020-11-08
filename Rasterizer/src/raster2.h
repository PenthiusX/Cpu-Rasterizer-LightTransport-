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
    Cam(){
        camPos = glm::vec3(0);
        camDir = glm::vec3(0,0,1);//direction camera is looking at
        camRight = glm::vec3(0);
        camDown = glm::vec3(0);
    }
    ~Cam(){};

    glm::vec3 camPos;
    glm::vec3 camDir;
    glm::vec3 camRight;
    glm::vec3 camDown;

    void lookAt(glm::vec3 lookatPos){
        camDir = glm::normalize(lookatPos - camPos);
    }
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

class Plane{
public:
    Plane(){}
    Plane(glm::vec3 p, glm::vec3 n , glm::vec3 co){
        pos = p;
        normal = n;
        color = co;
        distance = 0;
    }
    ~Plane(){}

    glm::vec3 pos;
    float distance;
    glm::vec3 normal;
    glm::vec3 color;

    double findIntersection(Ray ray){
        glm::vec3 rayDir = ray.dir;
        double a = glm::dot(rayDir,normal);
        if(a == 0){
            return -1.0;
        }
        else{
            double b = glm::dot(normal,(ray.origin + (normal * -distance)));
        }
    }
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
    unsigned int width = 400;
    unsigned int height = 400;

    //    int dpi = 72;
    int n = width * height;
    RGBType *pixels  = new RGBType[n];

    Cam c;
    c.camPos = glm::vec3(3,1.5,-4);
    c.lookAt(glm::vec3(0));
    c.camRight = glm::vec3(1,0,0);
    c.camDown = glm::vec3(0,-1,0);

    Light l;
    l.pos = glm::vec3(-7,10,-10);
    l.col = glm::vec3(1.0,1.0,1.0);

    Sphere sp(glm::vec3(0), 1.0, glm::vec3(0.5,0.5,0.0));

    Plane p(glm::vec3(0.0,-3.0,0.0), glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.5,0.0));
    int aspectRatio = width/height;

    double xamnt , yamnt;

    for(unsigned int x = 0 ; x < width ; x++)
    {
        for(unsigned int y = 0 ; y < height; y++)
        {

            unsigned int pos = y*width + x;

            pixels[pos].r = double(x) / (width-1);
            pixels[pos].g = double(y) / (height-1);
            pixels[pos].b = 0.25;

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

