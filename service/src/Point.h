#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vector>
#include <string>
#include <cstdio>

#define X 0
#define Y 1
#define Z 2

using namespace std;

class Point {
public:

    float coord[3];

    void clear();

    void setCoord(float x, float y, float z);
    void setCoord(Point point);

#if DEBUG
    string toString();
#endif
};

#endif
