#include "RayTracer.h"
#include <iostream>

using namespace glm;

bool Directional::isBlocked(vec3 pos, RayTracer* rt) {
    Ray r = Ray(pos, this->computeDirection(vec3(0, 0, 0)));
    return (rt->isIntersecting(r) >= 0.0);
}

float Directional::computeShade(vec3 v, vec3 normal) {
    vec3 shadow = this->computeDirection(vec3(0, 0, 0));
    float shade = dot(shadow, normal);
    return (shade > 0 ? shade : 0);
}

float Directional::attenuation(vec3 pos) {
    return 1.0;
}

bool Point::isBlocked(vec3 pos, RayTracer* rt) {
    vec3 dir = computeDirection(pos);
    Ray r = Ray(pos + 0.3f * dir, dir);
    float dist = length(pos - this->position);
    float inter = rt->isIntersecting(r);
    return (inter >= 0 && inter < dist);
}

float Point::computeShade(vec3 v, vec3 normal) {
    vec3 shadow = normalize(this->position - v);
    float shade = dot(shadow, normal);
    return (shade > 0 ? shade : 0);
}

float Point::attenuation(vec3 pos) {
    float dist = distance(this->position, pos);
    return 1 / (this->atten[0] + this->atten[1] * dist + this->atten[2] * dist * dist);
}