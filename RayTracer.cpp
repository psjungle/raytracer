#include "RayTracer.h"
#include <iostream>
#include <string>

using namespace glm;
using namespace std;

GridCell grid[5][5][5];

PixelColor RayTracer::traceRay(Ray r) {
    float closestDist = -1;
    Intersection closestInter;
    Shape* hitShape = nullptr;
    double t;
    // for (auto shape : scene->shapes) {
    //     shape->calculateIntersections(grid, r.origin.x, r.origin.y, r.origin.z, 800);
    // }
    for (int x = 0; x <= 4; ++x) {
        for (int y = 0; y <= 4; ++y) {
            for (int z = 0; z <= 4; ++z) {
                for (auto shape : grid[x][y][z].shapes) {
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
            }
        }
    }

    if (hitShape != nullptr) {
        return hitShape->findColor(closestInter);
    }

    return PixelColor(0, 0, 0);
}