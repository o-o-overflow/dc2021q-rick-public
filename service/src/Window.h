#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <cmath>

class Window {
private:
    int width;
    int height;

    int textCoordX,textCoordY;

    int helpTextCoordX,helpTextCoordY;

public:

    bool isTextActive;

    Window();
    Window(int width, int height);

    int getWidth();
    int getHeight();

    void setWidth(int width);
    void setHeight(int height);

    int getCenterX();
    int getCenterY();

    void zoomIn();
    void zoomOut();
    float getZoom();

    int getTextCoordX();
    int getTextCoordY();

    int getHelpTextCoordX();
    int getHelpTextCoordY();
};

#endif
