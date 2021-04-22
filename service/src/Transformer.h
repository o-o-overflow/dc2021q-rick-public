#ifndef _TRANSFORMER_H_
#define _TRANSFORMER_H_

#include <vector>

#include <GL/freeglut.h>

#define X 0
#define Y 1
#define Z 2

using namespace std;

class Transformer {
    
    public:
        static void translate(float tx,float ty,float tz);
        static void translate(vector<float> traslation);

        static void rotate(float rx,float ry,float rz);
        static void rotate(vector<float> rotation);

        static void scale(float sx,float sy,float sz);
        static void scale(vector<float> scalation);
};

#endif
