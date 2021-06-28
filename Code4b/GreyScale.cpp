#include "GreyScale.h"

GreyScale::GreyScale(int h, int w) {
#ifndef NDEBUG // no debug
    if (w < 0 || h < 0) greyScaleError("Positive Anzahl der Pixel in horiz./vert. Richtung!");
#endif
    height = h;
    width = w;
    pixels = new float[width * height];
    for (int i = 0; i < width * height; i++) {
        pixels[i] = 0;
    }
}

GreyScale::~GreyScale(){
    delete[] pixels;
}


GreyScale& GreyScale::resize(int h, int w) {
#ifndef NDEBUG
    if (w < 0 || h < 0) greyScaleError("Positive Anzahl der Pixel in horiz./vert. Richtung!");
#endif

    delete[] pixels;
    height = h;
    width = w;
    pixels = new float[width * height];
    for (int i = 0; i < width * height; i++) {
        pixels[i] = 0;
    }
    return *this;
}


float& GreyScale::operator()(int x, int y) {

    if (x >= 0 && x < width && y >= 0 && y < height)
        return pixels[y * width + x];
    else if (x == -1)
        return (*this)(0, y);
    else if (x == width)
        return (*this)(width-1, y);
    else if (y == -1)
        return (*this)(x, 0);
    else if (y == height)
        return (*this)(x, height-1);
    else
        exit(1);
}

float GreyScale::operator()(int x, int y) const {

    if (x >= 0 && x < width && y >= 0 && y < height)
        return pixels[y * width + x];
    else if (x == -1)
        return (*this)(0, y);
    else if (x == width)
        return (*this)(width-1, y);
    else if (y == -1)
        return (*this)(x, 0);
    else if (y == height)
        return (*this)(x, height-1);
    else
        exit(1);
}


GreyScale& GreyScale::operator=(const GreyScale& g) {
    if (this->getWidth() != g.getWidth() || this->getHeight() != g.getHeight()) {
        // delete[] pixels;
        // this->resize(g.width, g.height);
        this->resize(g.height, g.width);
    }

    // else {
        for (int i = 0; i < g.width * g.height; i++) {
            this->pixels[i] = g.pixels[i];
        } 
    // }

    return *this;
}

GreyScale& GreyScale::operator+=(const GreyScale& g) {
#ifndef NDEBUG
    if (this->getWidth() != g.getWidth() || this->getHeight() != g.getHeight()) {
        greyScaleError("Inkompatibele Dimension!");
    }
#endif

    for (int i = 0; i < g.width * g.height; i++) {
        this->pixels[i] += g.pixels[i];
    } 

    return *this;
}

GreyScale& GreyScale::operator-=(const GreyScale& g) {
#ifndef NDEBUG
    if (this->getWidth() != g.getWidth() || this->getHeight() != g.getHeight()) {
        greyScaleError("Inkompatibele Dimension!");
    }
#endif

    for (int i = 0; i < g.width * g.height; i++) {
        this->pixels[i] -= g.pixels[i];
    } 

    return *this;
}



void GreyScale::greyScaleError(const char str[]) {
    std::cerr << "\nGreyScale Error: " << str << '\n' << std::endl;
    std::abort();
}

