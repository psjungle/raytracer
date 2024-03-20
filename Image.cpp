#include "RayTracer.h"
#include "FreeImage.h"
#include <iostream>
#include <string>

using namespace std;

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    this->pixels = vector< vector<PixelColor> > (height, vector<PixelColor>(width, PixelColor(0, 0, 0)));
}

void Image::setPixel(int x, int y, double r, double g, double b) {
    this->pixels[y][x].r = r;
    this->pixels[y][x].g = g;
    this->pixels[y][x].b = b;
}

void Image::saveImage(string path) {
    FreeImage_Initialise();
    FIBITMAP* image = FreeImage_Allocate(this->width, this->height, 24);

    RGBQUAD c;
    // starting from top left
    for (int i = this->height - 1; i >= 0; i--) {
        for (int j = 0; j < this->width; j++) {
            c.rgbRed = this->pixels[i][j].b * 255;
            c.rgbGreen = this->pixels[i][j].g * 255;
            c.rgbBlue = this->pixels[i][j].r * 255;
            cout << i << " " <<  j << " " << this->pixels[i][j].r * 255 << " " << this->pixels[i][j].g * 255 << " " << this->pixels[i][j].b * 255 << endl;
            FreeImage_SetPixelColor(image, j, i, &c);
        }
    }
    FreeImage_Save(FIF_PNG, image, path.c_str(), 0);
}