#include <RayTracer.h>
#include <FreeImage.h>

using namespace std;

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    this->pixels = vector<vector<Pixel>> (height, vector<Pixel>(width, Pixel(0, 0, 0)));
}

void Image::setPixel(int x, int y, double r, double g, double b) {
    this->pixels[y][x].r += r;
    this->pixels[y][x].g += g;
    this->pixels[y][x].b += b;
}

void Image::saveImage(string path) {
    FreeImage_Initialise();
    FIBITMAP* image = FreeImage_Allocate(this->width, this->height, 24);

    for (int i = 0; i < this->height; i++) {
        RGBQUAD c;
        for (int j = 0; j < this->width; j++) {
            c.rgbRed = this->pixels[i][j].r * 255;
            c.rgbGreen = this->pixels[i][j].g * 255;
            c.rgbBlue = this->pixels[i][j].b * 255;
            FreeImage_SetPixelColor(image, i, j, &c);
        }
    }
    FreeImage_Save(FIF_PNG, image, path.c_str(), 0);
}