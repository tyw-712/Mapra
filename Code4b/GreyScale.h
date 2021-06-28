#ifndef GREYSCALE_H // GreyScale.h nicht doppelt benutzen
#define GREYSCALE_H

#include "unit.h"

#include <fstream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <arpa/inet.h>
#include <iostream>
#include "htree.h"
class GreyScale {
    private:
        int width, height;
        float* pixels;      
        static enum formatType {P2, P5, MHa, MHb} format;
        static int readInt(std::istream& s);
        static int readBit(std::istream& s, char& byte, int& bitPos);
        static int decode(std::istream &s,  char& byte, int& bitPos, Node& root);
        static int writeBit(std::ostream& o, char& byte, int bitVal, int& bitPos);
        static int writeBitFlush(std::ostream& o, char& byte, int& bitPos);
        static std::istream & readP2(std::istream& s, GreyScale& g);
        static std::istream & readP5(std::istream& s, GreyScale& g);
        static std::istream & readMHa(std::istream& s, GreyScale& g);
        static std::istream & readMHb(std::istream& s, GreyScale& g);
        static std::ostream & writeP2(std::ostream& o, const GreyScale& g);
        static std::ostream & writeP5(std::ostream& o, const GreyScale& g);
        static std::ostream & writeMHa(std::ostream& o, const GreyScale& g);
        static std::ostream & writeMHb(std::ostream& o, const GreyScale& g);
        static void greyScaleError(const char str[]);  // Fehlermeldung ausgeben


    public:    
        explicit GreyScale(int w = 0, int h = 0); // Konstruktor
        // explicit contructor: cannot be used for implicit conversions and copy-initialization
        ~ GreyScale(); // Destruktor

        float& operator()(int x, int y);       // Standard Zugriff auf GreyScaleElemente(Pixel)
        float operator()(int x, int y) const;  // Zugriff auf auf Elemente eines const GresyScales

        static void setFormat(int);

        GreyScale& resize(int h, int w);

        int getWidth() const {
            return width;
        }

        int getHeight() const {
            return height;
        }

        GreyScale& operator=(const GreyScale& g);   // Zuweisung
        GreyScale& operator+=(const GreyScale& g);  // Zuweisungen mit arithm. Operation
        GreyScale& operator-=(const GreyScale& g);

        GreyScale binarize(float c) const;
        GreyScale blur() const;
        GreyScale clamp() const;
        GreyScale contrast() const;
        GreyScale convolve(const float mask[], int size = 3) const;
        GreyScale kirsch() const;
        GreyScale laplace() const;
        GreyScale linTrans(float a, float b) const;
        GreyScale invert() const;
        GreyScale median() const;
        GreyScale sobel() const;

        GreyScale compTrans() const;
        GreyScale decompTrans() const;

        friend std::istream & operator>> (std::istream& s, GreyScale& g);
        friend std::ostream & operator<< (std::ostream& o, const GreyScale& g);
};

#endif