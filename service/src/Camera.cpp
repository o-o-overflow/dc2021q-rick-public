#include "Camera.h"

Camera::Camera() {
    resetView();
}


float Camera::getTX() { return tx_;}
float Camera::getTY() { return ty_;}
float Camera::getTZ() { return tz_;}
float Camera::getAngleX() {return angleX_;}
float Camera::getAngleY() {return angleY_;}
float Camera::getAngleZ() {return angleZ_;}

Point* Camera::getPosition() {
    Point * position = new Point();
    position->coord[X]=tx_;
    position->coord[Y]=ty_;
    position->coord[Z]=tz_;
    return position;
}

void Camera::setPosition(float tx, float ty, float tz) {
    if (tx!=(float)DONTCARE) tx_ = tx;
    if (ty!=(float)DONTCARE) ty_ = ty;
    if (tz!=(float)DONTCARE) tz_ = tz;
}
void Camera::setRotation(float angleX, float angleY, float angleZ) {
    if (angleX!=DONTCARE) angleX_ = angleX;
    if (angleY!=DONTCARE) angleY_ = angleY;
    if (angleZ!=DONTCARE) angleZ_ = angleZ;
}


#if DEBUG
void Camera::startView() {
    tx_=23;
    ty_=750;
    tz_=-390;
    angleX_=318;
    angleY_=235;
    angleZ_=0;
}
#endif

void Camera::resetView() {
    tx_=0;
    ty_=40;
    tz_=0;

    zoom_ = 60;

    angleX_=angleZ_=0;
    angleY_=270; // looking at the + of the x-axis
}

void Camera::moveForward(float dist) {
    float eyeMatrix[16];
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // In order to do the transformation, I use the opengl functions.
    // First I load the identity, then I traslate the current matrix
    // in the center of the eye (tx,ty,tz) and then I perform the rotation
    // around axes in the following order: around Y axis, around X axis and
    // eventually around the Z axis.
    // All the methods that move the camera act in this way!
    glLoadIdentity();
    glTranslatef(tx_,ty_,tz_);
    glRotatef(angleY_,0,1,0);
    glRotatef(angleX_,1,0,0);
    // Now the matrix coincides with the eye

    // I move the camera forward (parallel to the Z axis, that is setted
    // properly by the previous instructions
    glTranslatef(0,0,-dist);

    glGetFloatv(GL_MODELVIEW_MATRIX,eyeMatrix);

    glPopMatrix();

    tx_ = eyeMatrix[12];
    ty_ = eyeMatrix[13];
    tz_ = eyeMatrix[14];

    if (ty_<2.5) ty_=2.5;
}

void Camera::moveBackward(float dist) {
    float eyeMatrix[16];
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(tx_,ty_,tz_);
    glRotatef(angleY_,0,1,0);
    glRotatef(angleX_,1,0,0);

    glTranslatef(0,0,dist);

    glGetFloatv(GL_MODELVIEW_MATRIX,eyeMatrix);

    glPopMatrix();

    tx_ = eyeMatrix[12];
    ty_ = eyeMatrix[13];
    tz_ = eyeMatrix[14];
}

// it moves the camera forward, but it keeps the user on the XZ plane.
// The user can NOT fly with this method
void Camera::moveForwardOnXZ(float dist) {
    float eyeMatrix[16];
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(tx_,ty_,tz_);
    glRotatef(angleY_,0,1,0);

    glTranslatef(0,0,-dist);

    glGetFloatv(GL_MODELVIEW_MATRIX,eyeMatrix);

    glPopMatrix();

    tx_ = eyeMatrix[12];
    ty_ = eyeMatrix[13];
    tz_ = eyeMatrix[14];
}

// it moves the camera backward, but it keeps the user on the XZ plane.
// The user can NOT fly with this method
void Camera::moveBackwardOnXZ(float dist) {
    float eyeMatrix[16];
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(tx_,ty_,tz_);
    glRotatef(angleY_,0,1,0);

    glTranslatef(0,0,dist);

    glGetFloatv(GL_MODELVIEW_MATRIX,eyeMatrix);

    glPopMatrix();

    tx_ = eyeMatrix[12];
    ty_ = eyeMatrix[13];
    tz_ = eyeMatrix[14];
}

// it moves the camera on the left
void Camera::moveLeft(float dist) {
    float eyeMatrix[16];
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(tx_,ty_,tz_);
    glRotatef(angleY_,0,1,0);
    glRotatef(angleX_,1,0,0);

    glTranslatef(-dist,0,0);

    glGetFloatv(GL_MODELVIEW_MATRIX,eyeMatrix);

    glPopMatrix();

    tx_ = eyeMatrix[12];
    ty_ = eyeMatrix[13];
    tz_ = eyeMatrix[14];
}

// it moves the camera on the right
void Camera::moveRight(float dist) {
    float eyeMatrix[16];
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(tx_,ty_,tz_);
    glRotatef(angleY_,0,1,0);
    glRotatef(angleX_,1,0,0);

    glTranslatef(dist,0,0);

    glGetFloatv(GL_MODELVIEW_MATRIX,eyeMatrix);

    glPopMatrix();

    tx_ = eyeMatrix[12];
    ty_ = eyeMatrix[13];
    tz_ = eyeMatrix[14];
}

// it moves the camera up
void Camera::moveUp(float dist) {
    ty_+=dist;
}

// it moves the camera down
void Camera::moveDown(float dist) {
    ty_-=dist;
}

// it moves the camera up, but the height is kept below a certain threshold
void Camera::moveUpWithLock(float dist) {
    ty_+=dist;
    if (ty_>50) ty_=50;
    cout << "ty: " << ty_ << endl;
}

// it moves the camera down, but the height is kept up a certain threshold
void Camera::moveDownWithLock(float dist) {
    ty_-=dist;
    if (ty_<4) ty_=4;
}

// it makes the camera to look up
void Camera::lookUp(float angle) {
    angleX_+=angle;
    if (angleX_<0) angleX_=360;
    if (angleX_>360) angleX_=0;
}

// it makes the camera to look down
void Camera::lookDown(float angle) {
    angleX_-=angle;
    if (angleX_<0) angleX_=360;
    if (angleX_>360) angleX_=0;
}

// it makes the camera to look up (but the angle is kept below a certain value)
void Camera::lookUpWithLock(float angle) {
    angleX_+=angle;
    if (angleX_<-60) angleX_=-60;
    if (angleX_>60) angleX_=60;
}

// it makes the camera to look down (but the angle is kept above a certain value)
void Camera::lookDownWithLock(float angle) {
    angleX_-=angle;
    if (angleX_<-60) angleX_=-60;
    if (angleX_>60) angleX_=60;
}

// it makes the camera to look left
void Camera::lookLeft(float angle) {
    angleY_+=angle;
    if (angleY_<0) angleY_=360;
    if (angleY_>360) angleY_=0;
}

// it makes the camera to look right
void Camera::lookRight(float angle) {
    angleY_-=angle;
    if (angleY_<0) angleY_=360;
    if (angleY_>360) angleY_=0;
}

// it makes the camera to rotate around the Z axis on the left
void Camera::rollLeft(float angle) {
    angleZ_+=angle;
    if (angleZ_<0) angleZ_=360;
    if (angleZ_>360) angleZ_=0;
}

// it makes the camera to rotate around the Z axis on the right
void Camera::rollRight(float angle) {
    angleZ_-=angle;
    if (angleZ_<0) angleZ_=360;
    if (angleZ_>360) angleZ_=0;
}


#if DEBUG
string Camera::toString() {
    stringstream ss;
    string sMode="";

    ss << fixed;
    ss.precision(2);
    ss << "POS: (" << tx_ << "," << ty_ << "," << tz_ << "), ANGLE: (" << angleX_ << ", " << angleY_ << "," << angleZ_ << ")";
    return ss.str();
}
#endif

// it sets the camera to the identity
void Camera::setIdentity() {
    tx_=ty_=tz_=0;
    angleX_=angleY_=angleZ_=0;
}


// zoom out
void Camera::zoomOut() {
    zoom_+=1;
    if (zoom_>70) zoom_=70;
}

// zoom in
void Camera::zoomIn() {
    zoom_-=1;
    if (zoom_<10) zoom_=10;
}

// get the current zoom value
float Camera::getZoom() { return zoom_; }
