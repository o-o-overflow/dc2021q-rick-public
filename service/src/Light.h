#ifndef LIGHT_H_
#define LIGHT_H_

#include "Color.h"
#include "Point.h"
#include "Vector.h"

class Light {
private:
    Color ambientColor_;
    Color diffusionColor_;
    Point position_;
    Vector direction_;
    int index_;

public:

    bool isPositional;

    static int light_number;

    Light();
    Light(int index);

    int getIndex();

    Color getAmbientColor();
    void setAmbientColor(Color color);
    void setAmbientColor(float red, float green, float blue);

    Color getDiffusionColor();
    void setDiffusionColor(Color color);
    void setDiffusionColor(float red, float green, float blue);

    Point getPosition();
    void setPosition(Point point);
    void setPosition(float x, float y, float z);

    Vector getDirection();
    void setDirection(Vector direction);
    void setDirection(float x, float y, float z);

};

#endif
