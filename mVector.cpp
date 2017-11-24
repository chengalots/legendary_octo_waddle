//
//  mVector.cpp
//  SDL2_potato3
//
//  Created by pow on 2017-09-11.
//  Copyright © 2017 potato. All rights reserved.
//

#include "mVector.hpp"

mVector::mVector() {

    _dx = _dy = _theta = r = 0;
}

mVector::mVector(double dx, double dy, bool polarForm) {

    if(polarForm) {
        r = dx;
        _theta = dy;
        _dx = r * cos(_theta);
        _dy = r * sin(_theta);
    }
    else {
        _dx = dx;
        _dy = dy;
        r = sqrt((_dx * _dx) + (_dy * _dy));
        _theta = acos(dx / r);
    }
}

double mVector::dx() { return _dx; }

double mVector::dy() { return _dy; }

double mVector::magnitude() { return r; };

double mVector::theta() { return _theta; }

void mVector::setDx(double dx) {

    _dx = dx;
    r = (_dx * _dx) + (_dy * _dy);
    _theta = acos(_dx / r);
}

void mVector::setDy(double dy) {

    _dy = dy;
    r = (_dx * _dx) + (_dy * _dy);
    _theta = asin(_dy / r);
}
void mVector::setMagnitude(double magnitude) {

    r = magnitude;
    _dx = r * cos(_theta);
    _dy = r * sin(_theta);
}

void mVector::setTheta(double theta) {

    _theta = theta;
    _dx = r * cos(_theta);
    _dy = r * sin(_theta);
}
