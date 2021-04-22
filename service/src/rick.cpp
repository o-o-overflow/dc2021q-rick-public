#include <iostream>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <vector>
#include <math.h>
#include <GL/freeglut.h>

#include "Game.h"

#include "Drawer.h"
#include "Camera.h"
#include "Scene.h"
#include "Object.h"
#include "Window.h"

#include "handler.h"

using namespace std;


void init(int argc, char **argv);
void initRendering();
void setHandlers();
void startTimers();
void printHelp();

Game *game;


int main (int argc, char **argv) {
    init(argc,argv);
    initRendering();
    setHandlers();

    game = new Game();
    game->fetchNewLevel();

    startTimers();
    glutMainLoop();
    return 0;
}

void init (int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(30,30);
    glutCreateWindow("rick");

    glutSetCursor(GLUT_CURSOR_NONE);
}
    
void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glShadeModel(GL_SMOOTH); //Enable smooth shading
}

void setHandlers() {
    glutDisplayFunc(displayHandler); // set the rendering function
    glutKeyboardFunc(keyDownCallback);
    glutKeyboardUpFunc(keyUpCallback);
    glutSpecialFunc(specialKeyDownCallback);
    glutSpecialUpFunc(specialKeyUpCallback);
    glutReshapeFunc(reshapeHandler); // set the reshape handler
    glutIgnoreKeyRepeat(1);
}

void startTimers() {
    glutTimerFunc(0, mainTimer, 0);
}
