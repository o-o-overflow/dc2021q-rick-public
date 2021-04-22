#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include "Point.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class BoundingBox {
private:
    // coordinates of the bounding box
    float xMin_,xMax_;
    float yMin_,yMax_;
    float zMin_,zMax_;

public:
    BoundingBox();
    void clear();

    BoundingBox * getEnlargedBoundingBox();

    void setCoordinates(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

    void computeBoundingBox(vector <Point> points);

    bool isInside(float cx, float cy, float cz);

#if DEBUG
    string toString();
#endif

};

#endif
