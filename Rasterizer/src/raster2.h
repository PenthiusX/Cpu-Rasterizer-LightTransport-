#pragma once

#include <QImage>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define RND (2.0*(double)rand()/RAND_MAX-1.0)
#define RND2 ((double)rand()/RAND_MAX)
#endif


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

struct hit_record {
    glm::vec3 p;
    glm::vec3 normal;
    double t;

    bool front_face;

    void set_face_normal(const Ray& r, const glm::vec3& outward_normal)
    {
        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
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

    bool hit(Ray &r,float t_min,float t_max,hit_record& rec){

            glm::vec3 oc = r.origin() - center;
            float a = glm::length(r.direction()) * glm::length(r.direction());
            float half_b = dot(oc, r.direction());
            float c = (glm::length(oc)*glm::length(oc)) - radius*radius;

            float discriminant = half_b*half_b - a*c;
            if (discriminant < 0) return false;
            float sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            float root = (-half_b - sqrtd) / a;
            if (root < t_min || t_max < root) {
                root = (-half_b + sqrtd) / a;
                if (root < t_min || t_max < root)
                    return false;
            }

            //record the info per hit
            rec.t = root;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            //record the normal inward or outward based on where the ray is ats
            glm::vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);

            return true;
    }

    glm::vec3 center;
    float radius;
    glm::vec3 color;
};
//-----------------------------------------------------------------------------------------------

float clip(int n, int lower, int upper) {
    return std::max(lower, std::min(n, upper));
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
float hit_sphere(const glm::vec3& center, float radius, const Ray& r) {
    glm::vec3 rdir = glm::normalize(r.direction());
    glm::vec3 oc = r.origin() - center;
    float a = glm::dot(rdir,rdir);
    float b = 2.0 * glm::dot(oc, rdir);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant > 0);

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant) ) / (2.0*a);
    }
}
//-----------------------------------------------------------------------------------------------
glm::vec3 traceColor(const Ray& r){

//the render order overlap is top down // first return will overlap the second
    float t = hit_sphere(glm::vec3(0,0,-1), 1.5, r);
    if (t > 0.0) {
        glm::vec3 N =  glm::normalize(r.at(t) - glm::vec3(0,0,-1));
        return glm::vec3(0.5)*(glm::vec3(1)+N);
    }

    if(hit_sphere(glm::vec3(0.5,1.0,-2.5),1.3,r) > 0)
    {
        return glm::vec3(0.5, 0.5, 0);
    }

    glm::vec3 unit_direction = glm::normalize(r.direction());
    float t1 = (unit_direction.y + 1) * 0.5;
    return glm::vec3(1.0-t1)*glm::vec3(1.0, 1.0, 1.0) + glm::vec3(t1)*glm::vec3(0.5, 0.7, 1.0);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void render()
{
    const float aspectRatio = 16.0/9.0;
    unsigned int width = 1024;
    unsigned int height = static_cast<int>(width / aspectRatio);

    QImage image(width,height, QImage::Format_RGB32);

    //Camera
    Cam c;
    c.origin = glm::vec3(0.0,0.0,3);
    c.horizontal = glm::vec3(width,0,0);
    c.vertical = glm::vec3(0,height,0);
    c.focalLength = width/2;

    uint frames = 10;
    for(uint t = 0 ; t < frames ; t++)
    {   //animation debug loop

        c.origin.x = t*0.1;
        glm::vec3 lowerLeftCorner = c.origin - (c.horizontal*glm::vec3(0.5)) - (c.vertical*glm::vec3(0.5)) - glm::vec3(0, 0, c.focalLength);

        for(/*unsigned int y = 0 ; y < height ; y++*/unsigned int y = height ; y > 0 ; y--)//starts from max heights // topLeft edge // and moves to 0 bottom left
        {
            for(unsigned int x = 0 ; x < width; x++)
            {
                float u = float(x) / (width);
                float v = float(y) / (height);

                glm::vec3 dir = glm::normalize(lowerLeftCorner + (glm::vec3(u)*c.horizontal) + (glm::vec3(v)*c.vertical) - c.origin);
                Ray r(c.origin,dir);
                glm::vec3 col = traceColor(r);

                //              Ray r(glm::vec3(x,y,2),glm::vec3(0,0,-1));
                //              glm::vec3 col = raycolor(r);

                int ir = static_cast<int>(255.999 * col.x);
                int ig = static_cast<int>(255.999 * col.y);
                int ib = static_cast<int>(255.999 * col.z);

                //image.setPixel(x,y, qRgb(clip((int)ir,0,255),clip((int)ig,0, 255),clip((int)ib,0, 255)));
                image.setPixel(x,y, qRgb(ir,ig,ib));
                //image.save("Finalsave.jpeg", 0, -1);
            }
        }

        //    saveBmp(width,height,pixels);
        image.save(QString::number(t) + "Ras2.jpeg", 0, -1);
        if(t == frames-1)
            qInfo() << "Debugbreak";
    }
}
//--- --------------------------------------------------------------------------------------------
}

