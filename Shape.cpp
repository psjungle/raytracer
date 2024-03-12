#include <RayTracer.h>

using namespace glm;

Pixel Shape::findColor(Intersection inter) {
    return this->ambient;
}

class Triangle : public Triangle {
    private:
        vec3 A;
        vec3 B;
        vec3 C;
    public:
        Triangle(vec3 A, vec3 B, vec3 C, PixelColor ambient) {
            this->A = A;
            this->B = B;
            this->C = C;
            this->ambient = ambient;
        }

        vec3 computeNormal(vec3 dir) {
            return normalize(cross((B - A), (C - A)));
        }

        Intersection isIntersecting(Ray r) {
            vec3 normal = this->computeNormal(vec3(0, 0, 0));
            float t = dot(normal, r.position - A) / dot(normal, r.dir) * -1;
            vec3 pos = r.position + t * r.dir;
            if (t >= 0 &&
                dot(cross(B - A, pos - A), normal) >= 0 &&
                dot(cross(C - B, pos - B), normal) >= 0  &&
                dot(cross(A - C, pos - C), normal) >= 0) {
                return Intersection(pos, normal);
            }
            return NULL;
        }

}

class Sphere : public Shape {
    private:
        vec3 center;
        float radius;
        mat4 M;
    public:
        Sphere(vec3 center, float radius, mat4 M, PixelColor ambient) {
            this->center = center;
            this->radius = radius;
            this->M = M;
            this->ambient = ambient;
        }

        vec3 computeNormal(vec3 dir) {
            mat4 invM = inverse(this->M);
            vec3 temp = vec3(invM * vec3(dir, 1)) - this->center;
            temp = vec3(transpose(invM) * vec4(temp, 0));
            return normalize(temp);
        }

        Intersection isIntersecting(Ray r) {
            Ray rt = r.transform(inverse(this->M));

            float a = dot(rt.dir, rt.dir);
            float b = dot(2 * rt.dir, rt.position - this->center);
            float c = dot(rt.pos - center, rt.pos - center) - (radius * radius);
            float discr = b * b - 4 * a * c;

            if (discr < 0) return NULL;

            float t;
            float t1 = -b + sqrt(discr) / (2 * a);
            float t2 = -b - sqrt(discr) / (2 * a);

            if (t1 > 0 && t2 > 0) {
                t = min(t1, t2);
            }
            else if (t1 > 0 || t2 > 0) {
                t = (t1 > 0) ? t1 : t2;
            }
            else {
                return NULL;
            }

            vec3 pos = rt.position + t * rt.dir;
            pos = vec3(this->M * vec4(pos, 1));
            vec3 normal = this->computeNormal(pos);
            return Intersection(pos, normal);
        }
}