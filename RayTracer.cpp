#include "RayTracer.h"
#include <iostream>
#include <string>

using namespace glm;
using namespace std;

float RayTracer::isIntersecting(Ray r) {
    float closestDist = -1;
    for (auto shape : this->scene->shapes) {
        Intersection currInter = shape->isIntersecting(r);
        if (currInter.shape != nullptr) {
            float currDist = distance(r.origin, currInter.position); 
            if (closestDist < 0 || currDist < closestDist) {
                closestDist = currDist;
            }
        }
    }
    return closestDist;
}

PixelColor RayTracer::traceRay(Ray r, int depth, Camera* camera) {
    if (depth == 0) {
        return PixelColor(0, 0, 0);
    }

    float closestDist = -1;
    Intersection closestInter;
    Shape* hitShape = nullptr;
    for (auto shape : this->scene->shapes) {
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

        vec3 normal = hitShape->computeNormal(closestInter.position);
        vec3 parallel = dot(normal, normalize(r.dir)) * normal;
        vec3 reflection = normalize(r.dir - 1.5f * parallel);
        PixelColor currColor = hitShape->findColor(closestInter, this->scene, this, camera);
        Ray newRay(closestInter.position + 1.0f * reflection, reflection);
        currColor.r += hitShape->specular.r * traceRay(newRay, depth - 1, camera).r;
        currColor.g += hitShape->specular.g * traceRay(newRay, depth - 1, camera).g;
        currColor.b += hitShape->specular.b * traceRay(newRay, depth - 1, camera).b;
        return currColor;
    }

    return PixelColor(0, 0, 0);
}