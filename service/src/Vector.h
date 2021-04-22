#ifndef VECTOR_H_
#define VECTOR_H_

#include "Point.h"

class Vector {
public:
    Vector();
    float coord[3];

    void setCoord(float x, float y, float z);
    void setCoord(Vector vector);

    static Vector crossProduct(Vector u, Vector v);
    static Vector findNormal(Point point1, Point point2, Point point3);

#if DEBUG
    string toString();
#endif
};

#endif
