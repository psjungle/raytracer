#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include "RayTracer.h"
#include "Transform.h"

using namespace glm;
using namespace std;

vector< pair<string, vector<float> > > parseFile(string path) {
    ifstream file(path);
    string line;
    vector< pair<string, vector<float> > > cmds;

    if (!file.is_open()) {
        cerr << "Failed to open: "  << path << endl;
        return cmds;
    }

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        string cmd;
        vector<float> vals;
        istringstream iss(line);

        iss >> cmd;
        float val;
        while (iss >> val) {
            vals.push_back(val);
        }

        cmds.push_back(make_pair(cmd, vals));
    }

    file.close();
    return cmds;

}

void rightMultiply(const mat4 &M, stack<mat4> &transfstack) {
    mat4 &T = transfstack.top();
    T = T * M;
}

int main(int argc, char* argv[]) {
    string inputFile = argv[1];
    size_t pos = inputFile.find(".");
    string outputFile = inputFile.substr(0, pos) + ".png";

    auto cmds = parseFile(inputFile);

    Scene* scene = new Scene();
    Camera* camera;
    PixelColor ambient(0.2, 0.2, 0.2);
    PixelColor diffuse(0, 0, 0);
    PixelColor specular(0, 0, 0);
    float shininess;
    PixelColor emission(0, 0, 0);
    vec3 atten(1, 0, 0);
    vector<vec3> vertices;
    int count = 0;

    stack<mat4> transfstack;
    transfstack.push(mat4(1));

    for (auto cmd : cmds) {
        string name = cmd.first;

        cout << name << endl;
        
        if (name == "size") {
            scene->width = cmd.second[0];
            scene->height = cmd.second[1];
        }

        else if (name == "camera") {
            vec3 cameraPosition = vec3(cmd.second[0], cmd.second[1], cmd.second[2]);
            vec3 lookAt = vec3(cmd.second[3], cmd.second[4], cmd.second[5]);
            vec3 upVector = vec3(cmd.second[6], cmd.second[7], cmd.second[8]);
            float fovy = cmd.second[9];
            camera = new Camera(cameraPosition, upVector, lookAt, fovy, scene->width, scene->height);
        }

        else if (name == "maxdepth") {
            scene->maxDepth = cmd.second[0];
        }

        // Materials and Shapes

        else if (name == "maxverts") {
            vertices = vector<vec3>((int) cmd.second[0]);
        }

        else if (name == "vertex") {
            vertices[count] = vec3(cmd.second[0], cmd.second[1], cmd.second[2]);
            count++;
        }

        else if (name == "ambient") {
            ambient.r = cmd.second[0];
            ambient.g = cmd.second[1];
            ambient.b = cmd.second[2];
        }

        else if (name == "diffuse") {
            diffuse.r = cmd.second[0];
            diffuse.g = cmd.second[1];
            diffuse.b = cmd.second[2];
        }

        else if (name == "specular") {
            specular.r = cmd.second[0];
            specular.g = cmd.second[1];
            specular.b = cmd.second[2];
        }

        else if (name == "shininess") {
            shininess = cmd.second[0];
        }

        else if (name == "emission") {
            emission.r = cmd.second[0];
            emission.g = cmd.second[1];
            emission.b = cmd.second[2];
        }

        else if (name == "tri") {
            vec3 A = vec3(transfstack.top() * vec4(vertices[cmd.second[0]], 1));
            vec3 B = vec3(transfstack.top() * vec4(vertices[cmd.second[1]], 1));
            vec3 C = vec3(transfstack.top() * vec4(vertices[cmd.second[2]], 1));
            scene->shapes.push_back(new Triangle(A, B, C, ambient, diffuse, specular, shininess, emission));
        }

        else if (name == "sphere") {
            vec3 center = vec3(cmd.second[0], cmd.second[1], cmd.second[2]);
            float radius = cmd.second[3];
            scene->shapes.push_back(new Sphere(center, radius, transfstack.top(), ambient, diffuse, specular, shininess, emission));
        }
 
        // Transformations

        else if (name == "translate") {
            mat4 M = Transform::translate(cmd.second[0], cmd.second[1], cmd.second[2]);
            rightMultiply(M, transfstack);
        }

        else if (name == "rotate") {
            mat3 rot = Transform::rotate(cmd.second[3], vec3(cmd.second[0], cmd.second[1], cmd.second[2]));
            mat4 M = mat4(rot);
            M[3][3] = 1.0;
            rightMultiply(M, transfstack);
        }

        else if (name == "scale") {
            mat4 M = Transform::scale(cmd.second[0], cmd.second[1], cmd.second[2]);
            rightMultiply(M, transfstack);
        }

        else if (name == "pushTransform") {
            transfstack.push(transfstack.top());
        }

        else if (name == "popTransform") {
            transfstack.pop();
        }

        // Lighting

        else if (name == "attenuation") {
            atten[0] = cmd.second[0];
            atten[1] = cmd.second[1];
            atten[2] = cmd.second[2];
        }

        else if (name == "directional") {
            vec3 position = vec3(cmd.second[0], cmd.second[1], cmd.second[2]);
            position = vec3(transfstack.top() * vec4(position, 0));
            PixelColor color(cmd.second[3], cmd.second[4], cmd.second[5]);
            scene->lights.push_back(new Directional(color, position));
        }

        else if (name == "point") {
            vec3 position = vec3(cmd.second[0], cmd.second[1], cmd.second[2]);
            position = vec3(transfstack.top() * vec4(position, 0));
            PixelColor color(cmd.second[3], cmd.second[4], cmd.second[5]);
            scene->lights.push_back(new Point(color, position, atten));
        }

        else {
            cerr << "Invalid command: " << name << endl;
        }
    }

    Image* image = new Image(scene->width, scene->height);
    cout << "Raytracing ..." << endl;
    scene->render(image, camera);
    cout << "Saving image ..." << endl;
    image->saveImage(outputFile);
    cout << "Complete!" << endl;
}