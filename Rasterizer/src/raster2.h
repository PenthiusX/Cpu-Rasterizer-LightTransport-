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
    Cam(){}
    ~Cam(){}

    float viewportHeight = 2.0;
    float viewportWidth;
    float focalLength  = 1.0;

    glm::vec3 origin = glm::vec3(0);
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
//void saveBmp(unsigned int width , unsigned int height ,RGBType *data){
//    QImage image(width,height, QImage::Format_RGB32);

//    for (int x = 0; x < height; x++) {
//        for (int y = 0; y < width; y++) {
//            unsigned int pos = y * height + x;
//            RGBType rgb = data[pos];

//            double red = (data[pos].r)*255;
//            double green = (data[pos].g)*255;
//            double blue = (data[pos].b)*255;

//            unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};

//            image.setPixel(x, y, qRgb(clip((int)color[0],0,255),clip((int)color[1],0, 255),clip((int)color[2],0, 255)));
//        }
//    }
//    image.save("Finalsave.jpeg", 0, -1);
//}

//-----------------------------------------------------------------------------------------------


bool hit_sphere(const glm::vec3& center, float radius, const Ray& r) {
    glm::vec3 rdir = glm::normalize(r.direction());
    glm::vec3 oc = r.origin() - center;
    float a = glm::dot(rdir,rdir);
    float b = 2.0 * glm::dot(oc, rdir);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

glm::vec3 raycolor(const Ray& r){

    if(hit_sphere(glm::vec3(0.0,0.0,0.0),2.5,r)){
        return glm::vec3(1, 0, 0);
    }
    if(hit_sphere(glm::vec3(80.0,40.0,0.0),4.5,r)){
        return glm::vec3(0.5, 0.5, 0);
    }
//    float dist = glm::distance(glm::vec3(25,25,0),r.direction());
//    if(dist < 20.0){
//        return glm::vec3(1, 0, 0);
//    }
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float t = (unit_direction.y + 1) * 0.5;
    return glm::vec3(1.0-t)*glm::vec3(1.0, 1.0, 1.0) + glm::vec3(t)*glm::vec3(0.5, 0.7, 1.0);
}
void render()
{
    const float aspectRatio = 16.0/9.0;
    unsigned int width = 100;
    unsigned int height = 76;//static_cast<int>(width / aspectRatio);

    QImage image(width,height, QImage::Format_RGB32);

    for(uint t = 0 ; t < 100 ; t++)
    {   //animation debug loop

        //Camera
        Cam c;
        c.origin = glm::vec3(0.0,0.0,5);
        c.horizontal = glm::vec3(width,0,0);
        c.vertical = glm::vec3(0,height,0);
        c.focalLength = 25;

        glm::vec3 lowerLeftCorner = c.origin - (c.horizontal*glm::vec3(0.5)) - (c.vertical*glm::vec3(0.5)) - glm::vec3(0, 0, c.focalLength);

        for(/*unsigned int y = 0 ; y < height ; y++*/unsigned int y = height ; y > 0 ; y--)//starts from max heights // topLeft edge // and moves to 0 bottom left
        {
            for(unsigned int x = 0 ; x < width; x++)
            {
                float u = float(x) / (width);
                float v = float(y) / (height);

                glm::vec3 dir = lowerLeftCorner + (glm::vec3(u)*c.horizontal) + (glm::vec3(v)*c.vertical) - c.origin;
                Ray r(glm::vec3(x,y,0),dir);
                glm::vec3 col = raycolor(r);

//                Ray r(glm::vec3(x,y,0),glm::vec3(0,0,-1));
//                glm::vec3 col = raycolor(r);

                int ir = static_cast<int>(255.999 * col.x);
                int ig = static_cast<int>(255.999 * col.y);
                int ib = static_cast<int>(255.999 * col.z);

                //            image.setPixel(x,y, qRgb(clip((int)ir,0,255),clip((int)ig,0, 255),clip((int)ib,0, 255)));
                image.setPixel(x,y, qRgb(ir,ig,ib));
                //image.save("Finalsave.jpeg", 0, -1);
            }
        }

        //    saveBmp(width,height,pixels);
        image.save("Finalsave.jpeg", 0, -1);
        qInfo() << "Debugbreak";
    }
}
//--- --------------------------------------------------------------------------------------------
}

