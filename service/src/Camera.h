#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "GL/freeglut.h"
#include "math.h"
#include <string>
#include <sstream>
#include <iostream>

#include "Point.h"

#define DONTCARE (0x7fffffff)

using namespace std;

class Camera {
    private:
        


        // the state is stored with only six variables
        
        // three translations
        float tx_,ty_,tz_;
        
        // three angles around the three axes
        float angleY_;
        float angleX_;
        float angleZ_;

        float zoom_;

    public:

        Camera();
        
        float getTX();
        float getTY();
        float getTZ();
        float getAngleX();
        float getAngleY();
        float getAngleZ();

        Point* getPosition();
        void setPosition(float tx, float ty, float tz);
        void setRotation(float angleX, float angleY, float angleZ);

        void resetView();
        void startView();

        void setIdentity();

        void moveForward(float dist);
        void moveBackward(float dist);

        void moveForwardOnXZ(float dist);
        void moveBackwardOnXZ(float dist);

        void moveLeft(float dist);
        void moveRight(float dist);
        
        void moveUp(float dist);
        void moveDown(float dist);
        void moveUpWithLock(float dist);
        void moveDownWithLock(float dist);

        void lookUp(float angle);
        void lookDown(float angle);

        void lookUpWithLock(float angle);
        void lookDownWithLock(float angle);

        void lookLeft(float angle);
        void lookRight(float angle);
        
        void rollRight(float angle);
        void rollLeft(float angle);

        void doTranslation(float xt, float yt, float zt);
        void doRotation(float xr, float yr, float zr);

        void zoomIn();
        void zoomOut();
        float getZoom();
       
#if DEBUG
        string toString();
#endif


};

#endif
