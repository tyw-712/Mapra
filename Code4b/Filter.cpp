#include "GreyScale.h"
#include "unit.h"

#include <algorithm>
#include <cmath>
#include <vector>

GreyScale GreyScale::binarize(float c) const {
    GreyScale g(this->getHeight(), this->getWidth());
    for (int i = 0; i < this->getWidth() * this->getHeight(); i++) {
        if(this->pixels[i] > c) {
            g.pixels[i] = 1.0;
        }
        else {
            g.pixels[i] = 0.0;
        }
    } 

    return g;
}


GreyScale GreyScale::clamp() const {
    GreyScale g(height, width);
    for (int i = 0; i < width * height; i++) {
        if(this->pixels[i] > 1) {
            g.pixels[i] = 1;
        } else if(this->pixels[i] < 0) {
            g.pixels[i] = 0;
        } else {
            g.pixels[i] = this->pixels[i];
        }
    } 
    return g;
}


GreyScale GreyScale::convolve(const float mask[], int size) const {
    GreyScale g(this->getHeight(), this->getWidth());
    int s = (size - 1)/2;

    for (int h = 0; h < this->getHeight(); h++) {
        for (int w = 0; w < this->getWidth(); w++) {
            float sum = 0;
            for (int i = -s; i <= s; i++) {
                for (int j = -s ;j <= s; j++) {
                    sum += (*this)(h+i,w+j) * mask[(i+s)*size+(j+s)];
                }
            }
            g(h,w) = sum;
        }
    }

    return g;
}


GreyScale GreyScale::linTrans(float a, float b) const {
    GreyScale g(this->getHeight(), this->getWidth());
    for (int h = 0; h < this->getHeight(); h++) {
        for (int w = 0; w < this->getWidth(); w++) {
            g(w,h) = a * (*this)(w,h) + b;
        }
    }

    return g;
}


GreyScale GreyScale::contrast() const { // [min, max]-> [min/(max-min), max/(max-min)] -> [0, 1]
    // float max = *std::max_element(pixels, pixels + height*width); //*max_element(first_index, last_index)
    // float min = *std::min_element(pixels, pixels + height*width);
    float min = 1;
    float max = 0;

    for (int y = 0; y < width; y++)
        for (int x = 0; x < height; x++) {
            if ((*this)(x,y) > max)
                max = (*this)(x,y);
            if ((*this)(x,y) < min)
                min = (*this)(x,y);
        }
    float a = 1/(max-min); // normalise
    float b = -a * min; // shift
    return this->linTrans(a, b);
}


GreyScale GreyScale::invert() const {
    return linTrans(-1.0, 1);
}


GreyScale GreyScale::laplace() const {
    float mask[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    return convolve(mask, 3);
}


GreyScale GreyScale::blur() const {
    float mask[9] = {0, 0.2, 0, 0.2, 0.2, 0.2, 0, 0.2, 0};
    return convolve(mask, 3);
}


GreyScale GreyScale::kirsch() const {
    float mask[9] = {1, 3, 3, -1, 0 ,1, -3, -3, -1};
    return convolve(mask, 3);
}


GreyScale GreyScale::sobel() const {
    float DX[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    float DY[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    GreyScale X = convolve(DX, 3);
    GreyScale Y = convolve(DY, 3);
    GreyScale g(this->getHeight(), this->getWidth());

    for (int h = 0; h < this->getHeight(); h++) {
        for (int w = 0; w < this->getWidth(); w++) {
            g(h,w) = std::sqrt(std::pow(X(h,w),2) + std::pow(Y(h,w),2) );
        }
    }
    return g;
}


GreyScale GreyScale::median() const {
    GreyScale g(this->getHeight(), this->getWidth());
    int s = 1;
    for (int h = 0; h < this->getHeight(); h++) {
        for (int w = 0; w<this->getWidth(); w++) {
            std::vector<float> m;
            for (int i = -s; i <= s; i++) {
                for (int j = -s ;j <= s; j++) { 
                    m.push_back((*this)(h+i, w+j));
                }
            }
            std::sort(m.begin(), m.end());
            g(h,w) = m[4];
        }
    }

    return g;
}
