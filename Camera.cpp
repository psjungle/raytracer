#include <RayTracer.h>
#include <math.h>

using namespace glm;

Camera::Camera(vec3 cameraPosition, vec3 upVector, vec3 lookAt, float fovy, int width, int height) {
    this->width = width;
    this->height = height;

    w = normalize(cameraPosition - lookAt);
    u = normalize(cross(w, upVector));
    v = cross(w, u);

    float z  = tan(fovy / 2);
    z = (1 / z) * (height / 2);
    fovx = 2 * atan(width / 2);
}

Ray Camera::makeRay(int x, int y) {
    int hWidth = this->width / 2;
    int hHeight = this->height / 2;

    float alpha = -tan(this->fovx / 2) * (y - hWidth) / hWidth;
    float beta = tan(this->fovy / 2) * (hHeight / 2 - x)  / hHeight;
    vec3 dir = normalize(alpha * (this->u) + beta * (this->v) - w);
    return Ray(this->position, dir);
}