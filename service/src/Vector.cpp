#include "Vector.h"

Vector::Vector() {

}

void Vector::setCoord(float x, float y, float z) {
    coord[0]=x;
    coord[1]=y;
    coord[2]=z;
}
void Vector::setCoord(Vector vector) {
    coord[0]=vector.coord[0];
    coord[1]=vector.coord[1];
    coord[2]=vector.coord[2];
}


Vector Vector::crossProduct(Vector u, Vector v) {
    Vector result;
    float * cu = u.coord;
    float *cv = v.coord;

    result.coord[0] = cu[1]*cv[2] - cv[1]*cu[2];
    result.coord[1] = -cu[0]*cv[2] + cv[0]*cu[2];
    result.coord[2] = cu[0]*cv[1] - cv[0]*cu[1];

    return result;
}


Vector Vector::findNormal(Point point1, Point point2, Point point3) {
    Vector v1,v2;

    v1.coord[0] = point1.coord[0] - point2.coord[0];
    v1.coord[1] = point1.coord[1] - point2.coord[1];
    v1.coord[2] = point1.coord[2] - point2.coord[2];

    v2.coord[0] = point2.coord[0] - point3.coord[0];
    v2.coord[1] = point2.coord[1] - point3.coord[1];
    v2.coord[2] = point2.coord[2] - point3.coord[2];

    return crossProduct(v1, v2);
}


#if DEBUG
string Vector::toString() {
    char c[200];
    string s;

    sprintf(c, "Vector: %.2f %.2f %.2f",coord[0],coord[1],coord[2]);
    s = c;
    return s;
}
#endif
