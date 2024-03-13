#include "RayTracer.h"
#include <iostream>
#include <string>

using namespace glm;
using namespace std;

PixelColor RayTracer::traceRay(Ray r) {
    float closestDist = -1;
    Intersection closestInter;
    Shape* hitShape = nullptr;
    double t;
    for (auto shape : scene->shapes) {
        Intersection currInter = shape->isIntersecting(r);
        if (currInter.shape != nullptr) {
            float currDist = distance(r.origin, currInter.position); 
            if (closestDist < 0 || currDist < closestDist) {
                closestDist = currDist;
                closestInter = currInter;
                hitShape = shape;
            }
        }
    }

    if (hitShape != nullptr) {
        return hitShape->findColor(closestInter);
    }

    return PixelColor(0, 0, 0);
}