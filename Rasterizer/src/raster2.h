#pragma once

#include <QImage>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>

namespace Ras2{

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
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
class Light
{
public:
    Light(){}
    ~Light(){}

    glm::vec3 pos;
    glm::vec3 dir;

    glm::vec3 col;
};
//-----------------------------------------------------------------------------------------------
class Ray
{
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
struct hit_record
{
    glm::vec3 p;
    glm::vec3 normal;
    double t;
    //
    glm::vec3 color;//adi temp extra

    bool front_face;

    void set_face_normal(const Ray& r, const glm::vec3& outward_normal)
    {
        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};
//-----------------------------------------------------------------------------------------------
class Collider{//a pure virtual interface , can be extened to sphere , cube , tries(Mesh) , etc..

public:
    virtual ~Collider(){};
    virtual bool hit(Ray &r,float t_min,float t_max,hit_record& rec) = 0;
};

class Sphere : public Collider{//The sphere extension to Collider
public:
    Sphere(){}
    Sphere(glm::vec3 c, float r , glm::vec3 co){
        radius = r;
        center = c;
        color = co;
    }
    virtual ~Sphere(){}
    virtual bool hit(Ray &r,float t_min,float t_max, hit_record& rec)
    {
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
        rec.color = this->color;
        //record the normal inward or outward based on where the ray is ats
        glm::vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

    glm::vec3 center;
    float radius;
    glm::vec3 color;
};

class Scene{

private:
    std::vector<Collider*> colliders;

public:
    Scene(){}
    ~Scene(){
        for(auto c : colliders){delete c;}
        colliders.clear();
    }

    void add(Collider *c){
        colliders.push_back(c);
    }

    //Ray hit check on all scene objects
    bool isHit(Ray &r , float t_min, float t_max, hit_record& rec)
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = t_max;

        for (uint c = 0 ; c < colliders.size() ; c++)
        {
            if (colliders[c]->hit(r, t_min, closest_so_far, temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
};
//-----------------------------------------------------------------------------------------------

float clip(int n, int lower, int upper) {
    return std::max(lower, std::min(n, upper));
}
//-----------------------------------------------------------------------------------------------
//float hit_sphere(const glm::vec3& center, float radius, const Ray& r)
//{
//    glm::vec3 rdir = glm::normalize(r.direction());
//    glm::vec3 oc = r.origin() - center;
//    float a = glm::dot(rdir,rdir);
//    float b = 2.0 * glm::dot(oc, rdir);
//    float c = dot(oc, oc) - radius*radius;
//    float discriminant = b*b - 4*a*c;
//    //return (discriminant > 0);

//    if (discriminant < 0) {
//        return -1.0;
//    } else {
//        return (-b - sqrt(discriminant) ) / (2.0*a);
//    }
//}

glm::vec3 traceColor(Ray &r , Scene *s){
    hit_record rec;
    //the render order overlap is top down // first return will overlap the second
    if(s->isHit(r,0,infinity,rec)){
        return glm::vec3(0.5) * (rec.normal + rec.color);
    }

    //BackGround color
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float t1 = (unit_direction.y + 1) * 0.5;
    return glm::vec3(1.0-t1)*glm::vec3(1.0, 1.0, 1.0) + glm::vec3(t1)*glm::vec3(0.5, 0.7, 1.0);
}
//-----------------------------------------------------------------------------------------------

void render()
{
    const float aspectRatio = 16.0/9.0;
    unsigned int width = 1024;
    unsigned int height = static_cast<int>(width / aspectRatio);

    const int samplesPerPixel = 100;

    QImage image(width,height, QImage::Format_RGB32);

    //Camera
    Cam c;
    c.origin = glm::vec3(0.0,0.0,10);
    c.horizontal = glm::vec3(width,0,0);
    c.vertical = glm::vec3(0,height,0);
    c.focalLength = width/2;

    Scene *s = new Scene();
    s->add(new Sphere(glm::vec3(2,-2,-1.0),2.7,glm::vec3(0.5,0.5,0.0)));
    s->add(new Sphere(glm::vec3(0,-0.5,-5.0),0.5,glm::vec3(0.5,0.1,0.2)));
    s->add(new Sphere(glm::vec3(1,100,-3.0),100.2,glm::vec3(0.5,0.5,1.0)));

    uint aniFrames = 20;
    for(uint t = 0 ; t < aniFrames ; t++)
    {   //animation debug loop

        //y is inverted for some reason
        c.origin.y -= t*0.07;
         c.origin.x -= t*0.07;

        for(unsigned int y = 0 ; y < height ; y++/*unsigned int y = height ; y > 0 ; y--*/)//starts from max heights // topLeft edge // and moves to 0 bottom left
        {
            for(unsigned int x = 0 ; x < width; x++)
            {
                float u = float(x) / (width);
                float v = float(y) / (height);


                glm::vec3 lowerLeftCorner = c.origin - (c.horizontal*glm::vec3(0.5)) - (c.vertical*glm::vec3(0.5)) - glm::vec3(0, 0, c.focalLength);
                glm::vec3 dir = glm::normalize(lowerLeftCorner + (glm::vec3(u)*c.horizontal) + (glm::vec3(v)*c.vertical) - c.origin);
                Ray r(c.origin,dir);
                glm::vec3 col = traceColor(r,s);//check for intercetion and color that pixel

                int ir = static_cast<int>(255.999 * col.x);
                int ig = static_cast<int>(255.999 * col.y);
                int ib = static_cast<int>(255.999 * col.z);

                //image.setPixel(x,y, qRgb(clip((int)ir,0,255),clip((int)ig,0, 255),clip((int)ib,0, 255)));
                image.setPixel(x,y, qRgb(ir,ig,ib));
                //image.save("Finalsave.jpeg", 0, -1);
            }
        }
      //saveBmp(width,height,pixels);
        image.save(QString::number(t) + "Ras2.jpeg", 0, -1);
        if(t == aniFrames-1)
            qInfo() << "Debugbreak";
    }
    delete s;
}
//--- --------------------------------------------------------------------------------------------
}

