#include "Light.h"

Light::Light() {
}

Light::Light(int index) {
    index_ = index;
}


int Light::getIndex() {
    return index_;
}

Vector Light::getDirection()
{
    return direction_;
}

void Light::setDirection(Vector direction)
{
    direction_.setCoord(direction);
    isPositional=false;
}

void Light::setDirection(float x, float y, float z) {
    direction_.setCoord(x,y,z);
    isPositional=false;
}

Point Light::getPosition()
{
    return position_;
}

void Light::setPosition(Point position)
{
    position_.setCoord(position);
    isPositional=true;
}

void Light::setPosition(float x, float y, float z) {
    position_.setCoord(x,y,z);
    isPositional=true;
}


Color Light::getAmbientColor()
{
    return ambientColor_;
}

void Light::setAmbientColor(Color color)
{
    ambientColor_.setComponents(color.getRed(),color.getGreen(),color.getBlue());
}

void Light::setAmbientColor(float red, float green, float blue) {
    ambientColor_.setComponents(red,green,blue);
}

Color Light::getDiffusionColor()
{
    return diffusionColor_;
}

void Light::setDiffusionColor(Color color)
{
    diffusionColor_.setComponents(color.getRed(),color.getGreen(),color.getBlue());
}

void Light::setDiffusionColor(float red, float green, float blue) {
    diffusionColor_.setComponents(red,green,blue);
}
