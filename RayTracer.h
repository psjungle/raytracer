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
class GridCell;
class Light;

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

class GridCell {
public:
    std::vector<Shape*> shapes; // List of shapes in this cell

    void addShape(Shape* shape) {
        shapes.push_back(shape);
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
        PixelColor diffuse;
        PixelColor specular;
        float shininess;
        PixelColor emission;
        Shape(PixelColor ambient, PixelColor diffuse, PixelColor specular, float shininess, PixelColor emission) :
            ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), emission(emission) {};
        PixelColor findColor(Intersection inter, Scene* scene, RayTracer *rt, Camera* camera);
        virtual vec3 computeNormal(vec3 dir){ return vec3(0, 0, 0); };
        virtual Intersection isIntersecting(Ray r) { return Intersection(); };
        virtual void calculateIntersections(GridCell grid[5][5][5], int minX, int minY, int minZ, float cellSize)= 0;
};

class Triangle : public Shape {
    private:
        vec3 A;
        vec3 B;
        vec3 C;
    public:
        Triangle(vec3 A, vec3 B, vec3 C, PixelColor ambient, PixelColor diffuse, PixelColor specular, float shininess, PixelColor emission) :
            Shape(ambient, diffuse, specular, shininess, emission), A(A), B(B), C(C) {};
        vec3 computeNormal(vec3 dir) override;
        Intersection isIntersecting(Ray r) override;
        void calculateIntersections(GridCell grid[5][5][5], int minX, int minY, int minZ, float cellSize) override {
            float minXCoord = std::min(std::min(A.x, B.x), C.x);
            float maxXCoord = std::max(std::max(A.x, B.x), C.x);
            float minYCoord = std::min(std::min(A.y, B.y), C.y);
            float maxYCoord = std::max(std::max(A.y, B.y), C.y);
            float minZCoord = std::min(std::min(A.z, B.z), C.z);
            float maxZCoord = std::max(std::max(A.z, B.z), C.z);
            int minXIndex = static_cast<int>((minXCoord - minX) / cellSize);
            int maxXIndex = static_cast<int>((maxXCoord - minX) / cellSize);
            int minYIndex = static_cast<int>((minYCoord - minY) / cellSize);
            int maxYIndex = static_cast<int>((maxYCoord - minY) / cellSize);
            int minZIndex = static_cast<int>((minZCoord - minZ) / cellSize);
            int maxZIndex = static_cast<int>((maxZCoord - minZ) / cellSize);
            for (int x = std::max(0, minXIndex); x <= std::min(4, maxXIndex); ++x) {
                for (int y = std::max(0, minYIndex); y <= std::min(4, maxYIndex); ++y) {
                    for (int z = std::max(0, minZIndex); z <= std::min(4, maxZIndex); ++z) {
                        grid[x][y][z].shapes.push_back(this);
                    }
                }
            }
        }
};

class Sphere : public Shape {
    private:
        vec3 center;
        float radius;
        mat4 M;
    public:
        Sphere(vec3 center, float radius, mat4 M, PixelColor ambient, PixelColor diffuse, PixelColor specular, float shininess, PixelColor emission) :
            Shape(ambient, diffuse, specular, shininess, emission), center(center), radius(radius), M(M) {}
        vec3 computeNormal(vec3 dir) override;
        Intersection isIntersecting(Ray r) override;
        void calculateIntersections(GridCell grid[5][5][5], int minX, int minY, int minZ, float cellSize) override {
            int minXIndex = static_cast<int>((minX - this->radius) / cellSize);
            int maxXIndex = static_cast<int>((minX + this->radius) / cellSize);
            int minYIndex = static_cast<int>((minY - this->radius) / cellSize);
            int maxYIndex = static_cast<int>((minY + this->radius) / cellSize);
            int minZIndex = static_cast<int>((minZ - this->radius) / cellSize);
            int maxZIndex = static_cast<int>((minZ + this->radius) / cellSize);
            for (int x = std::max(0, minXIndex); x <= std::min(4, maxXIndex); ++x) {
                for (int y = std::max(0, minYIndex); y <= std::min(4, maxYIndex); ++y) {
                    for (int z = std::max(0, minZIndex); z <= std::min(4, maxZIndex); ++z) {
                        grid[x][y][z].shapes.push_back(this);
                    }
                }
            }
        }
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

class RayTracer {
    private:
    Scene* scene;
    public:
        RayTracer(Scene* scene){ this->scene = scene; };
        float isIntersecting(Ray r);
        PixelColor traceRay(Ray r, int maxDepth, Camera* camera);
};

class Scene {
    public:
    int width, height, maxDepth;
    vector<Shape*> shapes;
    vector<Light*> lights;
    Scene() : width(0), height(0), maxDepth(1), shapes(vector<Shape*>()), lights(vector<Light*>()) {};
    void render(Image* image, Camera* camera);
};

class Light {
    public:
        PixelColor color;
        vec3 position;
        Light(PixelColor color, vec3 position) : color(color), position(position) {};
        virtual vec3 computeDirection(vec3 pos) { return vec3(0, 0, 0); };
        virtual bool isBlocked(vec3 pos, RayTracer* rt) { return true; };
        virtual float computeShade(vec3 v, vec3 normal) { return 0.0; };
        virtual float attenuation(vec3 pos) { return 1.0; };
};

class Directional : public Light {
    public:
        Directional(PixelColor color, vec3 position) :
            Light(color, position) {};
        
        vec3 computeDirection(vec3 pos) {
            return normalize(this->position);
        }

        bool isBlocked(vec3 pos, RayTracer* rt) override;
        float computeShade(vec3 v, vec3 normal) override;
        float attenuation(vec3 pos) override;
};

class Point : public Light {
    public:
        vec3 atten;
        Point(PixelColor color, vec3 position, vec3 atten) :
            Light(color, position), atten(atten) {};

        vec3 computeDirection(vec3 pos) {
            return normalize(this->position - pos);
        }

        bool isBlocked(vec3 pos, RayTracer* rt) override;
        float computeShade(vec3 v, vec3 normal) override;
        float attenuation(vec3 pos) override;
};

class Camera {
    public:
        vec3 position;
        vec3 w;
        vec3 u;
        vec3 v;
        float fovx, fovy;
        int width, height;
        Camera(vec3 cameraPosition, vec3 upVector, vec3 lookAt, float fovy, int width, int height);
        Ray makeRay(int x, int y);
};



