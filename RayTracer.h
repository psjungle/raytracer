#include <math.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace glm;


class Pixel;
class Intersection;
class Shape;
class Image;
class Scene;
class Camera;
class Ray;
class RayTracer;

class Pixel {
    public:
        double r, g, b;
        Pixel(double r, double g, double b) {
            this->r = r;
            this->g = g;
            this->b = b;
        };
};

class Intersection {
    public:
    vec3 position;
    vec3 normal;
    Intersection() {
        this->position = vec3(0, 0, 0);
        this->normal = vec3(0, 0, 0);
    };
    Intersection(vec3 position, vec3 normal, Shape* shape) {
        this->position = position;
        this->normal = normal;
    };
};

class Shape {
    public:
        Shape(){};
        virtual Intersection isIntersecting(Ray r);
        virtual vec3 computeNormal(vec3 dir){ return Intersection(); };
        virtual Pixel findColor(Intersection inter) { return Pixel(0, 0, 0); };
};

class Image {
    private:
        int width, height;
        vector<vector<Pixel>> pixels;
    public:
        Image(int width, int height);
        void setPixel(int x, int y, double r, double g, double b);
        void saveImage(string path);
};

class Scene {
    public:
    int width, height;
    vector<Shape*> shapes;
    Scence(int width, int height) {
        this->width = width;
        this->height = height;
    }
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

class RayTracer {
    private:
    Scene* scene;
    public:
        RayTracer(Scene* scene){ this->scene = scene; };
        Pixel traceRay(Ray r);
};