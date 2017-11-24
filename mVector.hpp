//
//  mVector.hpp
//  SDL2_potato3
//
//  Created by pow on 2017-09-11.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef MVECTOR_HPP
#define MVECTOR_HPP
#include <cmath>

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
};

class mVector {
public:
    mVector();
    mVector(double dx, double dy, bool polarForm = false);
    double dx();
    double dy();
    double magnitude();
    double theta();
    void setDx(double dx);
    void setDy(double dy);
    void setMagnitude(double magnitude);
    void setTheta(double theta);
    
private:
    double _dx, _dy;
    double _theta;
    double r;
};

#endif
