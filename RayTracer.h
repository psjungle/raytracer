#include <math.h>
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;


class Pixel;
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
        }
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
        Ray(vec3 origin, vec3 dir);

};

class RayTracer {
    
    public:
        RayTracer(){};
        Pixel traceRay(Ray r);
};