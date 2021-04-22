#ifndef HANDLER_H_
#define HANDLER_H_

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <GL/freeglut.h>

#include "Game.h"
#include "State.h"
#include "Scene.h"
#include "Drawer.h"
#include "Window.h"

#include "utils.h"

void displayHandler(void);

void keyDownCallback(unsigned char key, int x, int y);
void keyUpCallback(unsigned char key, int x, int y);
void specialKeyDownCallback(int key, int x, int y);
void specialKeyUpCallback(int specialKey, int x, int y);

void reshapeHandler(int w, int h);

void openDoor(int);
void closeDoor(int);
bool checkDoorProximity();

void startPlaneTimer(int);

void keyMovementFunc();
void redisplayFunc();

void handleKeys();
bool handleKey(unsigned char key);
bool handleSpecialKey(int key);


void mainTimer(int value);


#endif
