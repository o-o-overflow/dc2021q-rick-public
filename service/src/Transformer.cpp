#include "Transformer.h"

void Transformer::translate(float tx, float ty, float tz) {
    glTranslatef(tx,ty,tz);
}
void Transformer::translate(vector<float> translation) {
    glTranslatef(translation[X],translation[Y],translation[Z]);
}

void Transformer::rotate(float rx, float ry, float rz) {
    glRotatef(ry,0.0,1.0,0.0);
    glRotatef(rx,1.0,0.0,0.0);
    glRotatef(rz,0.0,0.0,1.0);
}
void Transformer::rotate(vector<float> rotation) {
    glRotatef(rotation[Y],0.0,1.0,0.0);
    glRotatef(rotation[X],1.0,0.0,0.0);
    glRotatef(rotation[Z],0.0,0.0,1.0);
}

void Transformer::scale(float sx, float sy, float sz) {
    glScalef(sx,sy,sz);
}
void Transformer::scale(vector<float> scalation) {
    glScalef(scalation[X],scalation[Y],scalation[Z]);
}
