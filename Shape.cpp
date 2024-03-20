#include "RayTracer.h"
#include <iostream>
#include <string>

using namespace glm;
using namespace std;

PixelColor Shape::findColor(Intersection inter, Scene* scene, RayTracer* rt, Camera* camera) {

    if (inter.shape == nullptr) {
        return PixelColor(0, 0, 0);
    }

    PixelColor color(0, 0, 0);

    /*color.r = this->ambient.r + this->emission.r;
    color.g = this->ambient.g + this->emission.g;
    color.b = this->ambient.b + this->emission.b;
    return color;*/

    for (auto light : scene->lights) {
        if (!light->isBlocked(inter.position, rt)) {
            vec3 eyeDir = normalize(camera->position - inter.position);
            vec3 lightDir = light->computeDirection(inter.position);
            vec3 half0 = normalize(eyeDir + lightDir);
    
            float shade = light->computeShade(inter.position, inter.normal);
            PixelColor lambert(this->diffuse.r * shade, this->diffuse.g * shade, this->diffuse.b * shade);

            float nDotH = dot(inter.normal, half0);
            //cout << "nDotH: " << nDotH << endl;
            float pow = std::pow(nDotH, this->shininess);
            PixelColor phong(pow * this->specular.r, pow * this->specular.g, pow * this->specular.b);
            //cout << "PHONG: " << phong.r << " " << phong.g << " " << phong.b << endl;

            float atten = light->attenuation(inter.position);
            color.r = (lambert.r + phong.r) * atten;
            color.g = (lambert.g + phong.g) * atten;
            color.b = (lambert.b + phong.b) * atten;
        }
    }
    color.r += this->ambient.r + this->emission.r;
    color.g += this->ambient.g + this->emission.g;
    color.b += this->ambient.b + this->emission.b;
    return color;
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