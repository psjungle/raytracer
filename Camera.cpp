#include "RayTracer.h"
#include <math.h>
#include <iostream>
#include <string>

using namespace std;
using namespace glm;

Camera::Camera(vec3 cameraPosition, vec3 upVector, vec3 lookAt, float fovy, int width, int height) {
    this->position = cameraPosition;
    this->width = width;
    this->height = height;
    //cout << "Camera position: " << cameraPosition[0] << " " << cameraPosition[1] << " " << cameraPosition[2] << endl;
    this->w = normalize(cameraPosition - lookAt);
    this->u = normalize(cross(this->w, upVector));
    this->v = cross(this->w, this->u);

    //cout << this->v[0] << this->v[1] << this->v[2] << endl;

    this->fovy = radians(fovy);
    float z  = tan(this->fovy / 2);
    z = (1 / z) * (height / 2);
    this->fovx = 2 * atan((width / 2) / z);

    //cout << "fovx: " << this->fovx << endl;
    //cout << "fovy: " << this->fovy << endl;
}

Ray Camera::makeRay(int x, int y) {

    int hWidth = this->width / 2;
    int hHeight = this->height / 2;

    float alpha = -tan(this->fovx / 2) * ((x + 0.5) - hWidth) / hWidth;
    float beta = tan(this->fovy / 2) * (hHeight - (y + 0.5)) / hHeight;
    vec3 dir = normalize(alpha * (this->u) + beta * (this->v) - w);


    return Ray(this->position, dir);
}