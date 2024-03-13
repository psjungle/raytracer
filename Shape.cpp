#include "RayTracer.h"
#include <iostream>
#include <string>

using namespace glm;
using namespace std;

PixelColor Shape::findColor(Intersection inter) {
    return this->ambient;
}

vec3 Triangle::computeNormal(vec3 dir) {
    return normalize(cross(B - A, C - A));
}

Intersection Triangle::isIntersecting(Ray r) {
    vec3 normal = this->computeNormal(vec3(0, 0, 0));
    float t = dot(normal, r.origin - A) / dot(normal, r.dir) * -1;
    vec3 pos = r.origin + t * r.dir;

    if ((t >= 0) &&
        (dot(cross((B - A), (pos - A)), normal) >= 0) &&
        (dot(cross((C - B), (pos - B)), normal) >= 0)  &&
        (dot(cross((A - C), (pos - C)), normal) >= 0)) {
        return Intersection(pos, normal, this);
    }

    return Intersection();
}


vec3 Sphere::computeNormal(vec3 dir) {
    mat4 invM = inverse(this->M);
    vec3 temp = vec3(invM * vec4(dir, 1)) - this->center;
    temp = vec3(transpose(invM) * vec4(temp, 0));
    return normalize(temp);
}

Intersection Sphere::isIntersecting(Ray r) {
    Ray rt = r.transform(inverse(this->M));

    float a = dot(rt.dir, rt.dir);
    float b = dot((float) 2 * rt.dir, rt.origin - this->center);
    float c = dot(rt.origin - this->center, rt.origin - this->center) - (this->radius * this->radius);
    float discr = b * b - 4 * a * c;

    if (discr < 0) return Intersection();

    float t;
    float t1 = (-b + sqrt(discr)) / (2 * a);
    float t2 = (-b - sqrt(discr)) / (2 * a);

    if (t1 > 0 && t2 > 0) {
        t = std::min(t1, t2);
    }
    else if (t1 > 0 || t2 > 0) {
        t = (t1 > 0) ? t1 : t2;
    }
    else {
        return Intersection();
    }

    vec3 pos = rt.origin + t * rt.dir;
    pos = vec3(this->M * vec4(pos, 1));
    vec3 normal = this->computeNormal(pos);
    return Intersection(pos, normal, this);
}