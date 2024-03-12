#include <math.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace glm;


class PixelColor;
class Ray;
class Intersection;
class Shape;
class Image;
class Scene;
class Camera;
class RayTracer;

class PixelColor {
    public:
        double r, g, b;
        PixelColor(double r, double g, double b) {
            this->r = r;
            this->g = g;
            this->b = b;
        };
};

class Ray {
    public:
        vec3 origin, dir;
        Ray(vec3 origin, vec3 dir) {
            this->origin = origin;
            this->dir = dir;
        };
        Ray transform(mat4 M) {
            vec3 newOrigin = vec3(M * vec4(this->origin, 1));
            vec3 newDir = normalize(vec3(M * vec4(this->dir, 0)));
            return Ray(newOrigin, newDir);
        }
};

class Intersection {
    public:
    vec3 position;
    vec3 normal;
    Shape* shape;
    Intersection() {
        this->position = vec3(0, 0, 0);
        this->normal = vec3(0, 0, 0);
        this->shape = nullptr;
    };
    Intersection(vec3 position, vec3 normal, Shape* shape) {
        this->position = position;
        this->normal = normal;
        this->shape = shape;
    };
};

class Shape { 
    public:
        PixelColor ambient;
        Shape(PixelColor ambient) : ambient(ambient) {};
        PixelColor findColor(Intersection inter);
        virtual vec3 computeNormal(vec3 dir){ return vec3(0, 0, 0); };
        virtual Intersection isIntersecting(Ray r) { return Intersection(); };
};

class Triangle : public Shape {
    private:
        vec3 A;
        vec3 B;
        vec3 C;
    public:
        Triangle(vec3 A, vec3 B, vec3 C, PixelColor ambient) :
            Shape(ambient), A(A), B(B), C(C) {};
        vec3 computeNormal(vec3 dir) override;
        Intersection isIntersecting(Ray r) override;
};

class Sphere : public Shape {
    private:
        vec3 center;
        float radius;
        mat4 M;
    public:
        Sphere(vec3 center, float radius, mat4 M, PixelColor ambient) :
            Shape(ambient), center(center), radius(radius), M(M) {}
        vec3 computeNormal(vec3 dir) override;
        Intersection isIntersecting(Ray r) override;
};

class Image {
    private:
        int width, height;
        vector< vector<PixelColor> > pixels;
    public:
        Image(int width, int height);
        void setPixel(int x, int y, double r, double g, double b);
        void saveImage(string path);
};

class Scene {
    public:
    int width, height;
    vector<Shape*> shapes;
    Scene() : width(0), height(0), shapes(vector<Shape*>()) {};
    void render(Image* image, Camera* camera);
};


class Camera {
    private:
        vec3 position;
        vec3 w;
        vec3 u;
        vec3 v;
        float fovx, fovy;
        int width, height;
    public:
        Camera(vec3 cameraPosition, vec3 upVector, vec3 lookAt, float fovy, int width, int height);
        Ray makeRay(int x, int y);
};

class RayTracer {
    private:
    Scene* scene;
    public:
        RayTracer(Scene* scene){ this->scene = scene; };
        PixelColor traceRay(Ray r);
};