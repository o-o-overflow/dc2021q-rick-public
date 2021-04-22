#include "Window.h"

Window::Window() {
    width=800;
    height=600;
    isTextActive=true;
    textCoordX=236;
    textCoordY=283;
    helpTextCoordX=-380;
    helpTextCoordY=283;
}

Window::Window(int w, int h) {
    width=w;
    height=h;
    isTextActive=true;
    textCoordX=236;
    textCoordY=283;
    helpTextCoordX=-380;
    helpTextCoordY=283;
}


int Window::getWidth() { return width; }
int Window::getHeight() { return height; }
void Window::setWidth(int w) { width=w; }
void Window::setHeight(int h) { height=h; }


int Window::getCenterX() { return width/2.0; }
int Window::getCenterY() { return height/2.0; }


int Window::getTextCoordX() {return width/2 - 164;}
int Window::getTextCoordY() { return height/2-17; }
int Window::getHelpTextCoordX() { return -width/2+20; }
int Window::getHelpTextCoordY() { return height/2-17; }
