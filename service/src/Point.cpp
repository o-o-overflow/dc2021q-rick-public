#include "Point.h"


void Point::clear() { coord[0]=coord[1]=coord[2]=0; }

void Point::setCoord(float x, float y, float z) {
    coord[0]=x;
    coord[1]=y;
    coord[2]=z;
}
void Point::setCoord(Point point) {
    coord[0]=point.coord[0];
    coord[1]=point.coord[1];
    coord[2]=point.coord[2];
}


#if DEBUG
string Point::toString() {
    char c[200];
    string s;
    
    sprintf(c, "Point: %.2f %.2f %.2f",coord[0],coord[1],coord[2]);
    s = c;
    return s;
}
#endif
