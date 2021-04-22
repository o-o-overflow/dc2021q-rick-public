#ifndef SCENE_H_
#define SCENE_H_

#include "Light.h"
#include "Object.h"
#include "Formula.h"
#include "Node.h"
#include "Camera.h"


#define CHURCH_X 0
#define CHURCH_Y 0
#define CHURCH_Z 1000

#define LEVER_DEPTH 10
#define LEVER_WIDTH 10
#define LEVER_HEIGHT 80
#define LEVER_Y (-5)

#define BOX_DEPTH 100
#define BOX_WIDTH 200
#define BOX_HEIGHT 100
#define BOX_Y (BOX_HEIGHT/2)
#define EXTRA_SPACE 100
#define ROOT_X 300
#define ROOT_Z 0

#define RICK_SCALE_X 10
#define RICK_SCALE_Y 10
#define RICK_SCALE_Z 10


class Game;

class Scene {
public:
    vector<Object *> objects;
    vector<Object *> objectsNodes;
    map< pair<int, int>, vector <Object *> > objectsNodesByCoords;
    vector<Light*> lights;
    Object *nextLevelObject;
    Object *churchDoor;
    Object *rick;

    Color background_color;
    Color ambient_color;

    Scene();

    void clear();

    Color getBackgroundColor();
    void setBackgroundColor(Color color);

    Color getAmbientColor();

    vector <Object *> getObjects();
    vector <Object *> getObjectsNodes();
    vector <Light *> getLights();

    void createLights();

    void addLight(Light * light);
    void addObject(Object * object);
    void addObjectNode(Object * object);
    void addFormulaObjects(Game *g);

    void placeFormulaObjects(Game *g);
    bool buildScene(Game *g);

    bool checkForCollision(float cx, float cy, float cz);
    bool checkForCollision(Camera *camera);

    void openDoor();
    void closeDoor();
};

#endif
