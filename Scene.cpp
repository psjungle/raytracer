#include "RayTracer.h"
#include <iostream>
using namespace std;
void Scene::render(Image* image, Camera* camera) {
    RayTracer* raytracer = new RayTracer(this);
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            Ray ray = camera->makeRay(j, i);
            PixelColor p = raytracer->traceRay(ray, this->maxDepth, camera);
            cout << "Pixel " << i << " " << j << " " << p.r << " " << p.g << " " << p.b << endl;
            image->setPixel(j, i, p.r, p.g, p.b);
        }
    }
}