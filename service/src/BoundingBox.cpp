#include "BoundingBox.h"

BoundingBox::BoundingBox() {
    clear();
}

void BoundingBox::clear() {
    xMin_=xMax_=yMin_=yMax_=zMin_=zMax_=0;
}

BoundingBox* BoundingBox::getEnlargedBoundingBox() {
    BoundingBox * bb = new BoundingBox();
    bb->setCoordinates(xMin_-70,xMax_+70,yMin_-70,yMax_+70,zMin_-70,zMax_+70);
    return bb;
}

void BoundingBox::setCoordinates(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
    xMin_ = xMin;
    xMax_ = xMax;
    yMin_ = yMin;
    yMax_ = yMax;
    zMin_ = zMin;
    zMax_ = zMax;
}

void BoundingBox::computeBoundingBox(vector <Point> points) {
    float xMin,xMax,yMin,yMax,zMin,zMax;
    float x,y,z;
    float border = 10;

    xMin = xMax = points[0].coord[X];
    yMin = yMax = points[0].coord[Y];
    zMin = zMax = points[0].coord[Z];

    for (unsigned int j=1; j<points.size(); j++) {
        x = points[j].coord[X];
        y = points[j].coord[Y];
        z = points[j].coord[Z];

        if (x < xMin) xMin = x;
        else if (x > xMax) xMax = x;

        if (y < yMin) yMin = y;
        else if (y > yMax) yMax = y;

        if (z < zMin) zMin = z;
        else if (z > zMax) zMax = z;
    }

    xMin -= border;    xMax += border;
    yMin -= border;    yMax += border;
    zMin -= border;    zMax += border;

    setCoordinates(xMin,xMax,yMin,yMax,zMin,zMax);
}

bool BoundingBox::isInside(float cx, float cy, float cz) {
    if ( (cx < xMin_) || (cx > xMax_) ) return false;
    if ( (cy < yMin_) || (cy > yMax_) ) return false;
    if ( (cz < zMin_) || (cz > zMax_) ) return false;
    return true;
}


#if DEBUG
string BoundingBox::toString() {
    stringstream ss;
    ss << "bb: ";
    ss << "x: " << xMin_ << " " << xMax_;
    ss << " y: " << yMin_ << " " << yMax_;
    ss << " z: " << zMin_ << " " << zMax_ << endl;
    return ss.str();
}
#endif
