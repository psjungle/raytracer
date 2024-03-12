#include <math.h>
#include <glm/glm.hpp>

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
        int x, y, r, g, b;
        Pixel(int x, int y, int r, int g, int b) {
            this->x = x;
            this->y = y;
            this->r = r;
            this->g = g;
            this->b = b;
        }
};

class Image {

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
        Ray makeRay(Pixel* pixel);
};

class Ray {

    public:
        Ray(vec3 origin, vec3 dir);

};

class RayTracer {

};