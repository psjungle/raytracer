#include <RayTracer.h>

using namespace glm;

Pixel RayTracer::traceRay(Ray r) {
    float closestDist = INFINITY;
    Intersection closestInter;
    Shape* hitShape = nullptr;
    double t;
    for (auto shape : scene->shapes) {
        Intersection currInter = shape->isIntersecting(r);
        if (shape->isIntersecting(r)) {
            float currDist = distance(r->position, inter->position); 
            if (currDist < closestDist) {
                closestDist = currDist;
                closestInter = currInter;
                hitShape = shape;
            }
        }
    }

    if (hitShape) {
        return hitShape->findColor(closestInter);
    }
    return Pixel(0, 0, 0);
}