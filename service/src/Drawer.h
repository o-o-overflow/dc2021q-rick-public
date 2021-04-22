#ifndef DRAWER_H_
#define DRAWER_H_

#include <unistd.h>
#include <limits.h>
#include <map>
#include <string.h>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "Scene.h"
#include "Object.h"
#include "Transformer.h"
#include "Camera.h"
#include "Window.h"
#include "Light.h"
#include "Point.h"
#include "Vector.h"

#include <GL/freeglut.h>

#define CODE_LEN 16
#define PATTERN_LEN 64

class Game;

class Drawer {
private:
    Game *game;
    map <string, unsigned int> textureIDs_;
    map <string, unsigned int> textureCode2ID_;

    void drawAmbientColor(Color * color);
    void drawLight(Light * light);
    void drawObject(Object * object);

public:
    Drawer(Game *game);

    void clear();

    void loadTextures();
    void loadResource(unsigned int textureID, string textureCode);
    void drawScene();
    void drawText(string s, int x, int y);
    void drawLevelText();
    void drawDisconnected();
    void drawFlag();

    void setCameraProjection(Camera * camera, Window * window);
    void setCameraPosition(Camera * camera);
};

#endif
