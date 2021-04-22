#include "handler.h"


extern Game *game;


void displayHandler(void) {
    auto drawer = game->drawer;
    auto camera = game->camera;
    auto window = game->window;

    glPushMatrix();
    glLoadIdentity();

    drawer->setCameraProjection(camera, window);
    drawer->setCameraPosition(camera);

    drawer->drawScene();

    glPopMatrix();

    // swap the two buffer, in order to render the current scene!
    glutSwapBuffers();
}


void keyDownCallback(unsigned char key, int, int) {
    auto state = game->state;
    state->pressKey(key);
}

void keyUpCallback(unsigned char key, int, int) {
    auto state = game->state;
    state->unpressKey(key);
}

// special keys handler (key down)
void specialKeyDownCallback(int specialKey, int, int) {
    auto state = game->state;
    state->pressSpecialKey(specialKey);
}

// special keys handler (key up)
void specialKeyUpCallback(int specialKey, int, int) {
    auto state = game->state;
    state->unpressSpecialKey(specialKey);
}


// handle the reshape event
void reshapeHandler(int width, int height) {
    auto window = game->window;
    auto drawer = game->drawer;
    auto camera = game->camera;

    window->setWidth(width);
    window->setHeight(height);

    drawer->setCameraProjection(camera,window);
    drawer->setCameraPosition(camera);

    glViewport(0,0,width,height);
}


void handleKeys() {
    auto s = game->state;
    bool shouldUnpress;
    if (!s->pressedKeys.empty()) {
        for (auto it=s->pressedKeys.begin(); it!=s->pressedKeys.end(); ) {
            int key = *it;
            if (key & 0x80000000) {
                shouldUnpress = handleSpecialKey(key & 0x7fffffff);
            } else {
                shouldUnpress = handleKey(key);
            }
            if (shouldUnpress) {
                s->pressedKeys.erase(it);
            } else {
                it++;
            }
        }
    }
}


bool handleKey(unsigned char key) {
    auto camera = game->camera;
    auto state = game->state;
    auto scene = game->scene;

    float cx = camera->getTX();
    float cy = camera->getTY();
    float cz = camera->getTZ();
    vector <Object*> objects;
    vector <Polygon*> polygons;
    bool shouldUnpress = false;
    unsigned int SPEED = 5;
    switch (state->getMode()) {
    case MODE_NORMAL:
        switch (key) {
        case 'a': case 'A':
            for (unsigned int i=0; i<SPEED; i++) {
                camera->moveLeft(1);
                if (scene->checkForCollision(camera)) {
                    camera->moveLeft(-1);
                    break;
                }
            }
            break;
        case 'd': case 'D':
            for (unsigned int i=0; i<SPEED; i++) {
                camera->moveRight(1);
                if (scene->checkForCollision(camera)) {
                    camera->moveRight(-1);
                    break;
                }
            }
            break;
        case 'w': case 'W': {
            unsigned int speed = SPEED*2;
            if (state->withBoost()) speed *= 4;
            for (unsigned int i=0; i<speed; i++) {
                camera->moveForwardOnXZ(1);
                if (scene->checkForCollision(camera)) {
                    camera->moveForwardOnXZ(-1);
                    break;
                }
            }
            break;
        }
        case 's': case 'S': {
            unsigned int speed = SPEED*2;
            if (state->withBoost()) speed *= 4;
            for (unsigned int i=0; i<speed; i++) {
                camera->moveBackwardOnXZ(1);
                if (scene->checkForCollision(camera)) {
                    camera->moveBackwardOnXZ(-1);
                    break;
                }
            }
            break;
        }
#if DEBUG
    case MODE_FLY:
        switch (key) {
        case 'a': case 'A':
            for (unsigned int i=0; i<SPEED; i++) {
                camera->moveLeft(1);
                if (scene->checkForCollision(camera)) {
                    camera->moveLeft(-1);
                    break;
                }
            }
            break;
        case 'd': case 'D':
            for (unsigned int i=0; i<SPEED; i++) {
                camera->moveRight(1);
                if (scene->checkForCollision(camera)) {
                    camera->moveRight(-1);
                    break;
                }
            }
            break;
        case 'w': case 'W':
            for (unsigned int i=0; i<SPEED*2; i++) {
                camera->moveForward(1);
                if (scene->checkForCollision(camera)) {
                    camera->moveForward(-1);
                    break;
                }
            }
            break;
        case 's': case 'S':
            for (unsigned int i=0; i<SPEED*2; i++) {
                camera->moveBackward(1);
                if (scene->checkForCollision(camera)) {
                    camera->moveBackward(-1);
                    break;
                }
            }
            break;
        }
        break;
#endif
    }

    switch (key) {
#if DEBUG
    case '1':
        state->setMode(MODE_NORMAL);
        break;
    case '2':
        state->setMode(MODE_FLY);
        break;
    case 'r': case 'R':
        camera->resetView();
        break;
#endif
    case 27: // ESC key
        exit(0);
        break;
    case 'e': case 'E': {
        shouldUnpress = true;
        auto nodes = game->formula->nodes;
        auto n2o = game->nid2obj;
        bool hit = false;
        for (auto it=nodes.begin(); it!=nodes.end(); it++) {
            auto n = it->second;
            if (n->type != NODE_TYPE_INPUT) continue;

            auto o = n2o[n->id];
            if (o->isNear(cx, cy, cz)) {
                // cout << "near node_id " << it->first << endl;
                n->flipState();
                game->nodesStateDirty = true;
                hit = true;
                break;
            }
        }
        if (hit) break;
        auto nlo = game->scene->nextLevelObject;
        if (game->connected && nlo->isNear(cx, cy, cz)) {
            // cout << "near NLO" << endl;
            if (game->formula->root->isOn()) {
                game->submitInputs();
                game->fetchNewLevel();
            }
            break;
        }
        break;
    }
    }
    }

    return shouldUnpress;
}


bool handleSpecialKey(int key) {
    auto state = game->state;
    auto camera = game->camera;

    switch (state->getMode()) {
    case MODE_NORMAL:
        switch(key) {
        case GLUT_KEY_LEFT:
            camera->lookLeft(5);
            break;
        case GLUT_KEY_RIGHT:
            camera->lookRight(5);
            break;
        case GLUT_KEY_UP:
            camera->lookUpWithLock(2);
            break;
        case GLUT_KEY_DOWN:
            camera->lookDownWithLock(2);
            break;
        }
        break;
#if DEBUG
    case MODE_FLY:
        switch(key) {
        case GLUT_KEY_LEFT:
            camera->lookLeft(5);
            break;
        case GLUT_KEY_RIGHT:
            camera->lookRight(5);
            break;
        case GLUT_KEY_UP:
            camera->lookUp(2);
            break;
        case GLUT_KEY_DOWN:
            camera->lookDown(2);
            break;
        }
        break;
#endif
    }
    return false;
}


void redisplayFunc() {
    glutPostRedisplay();
}


void updateFormulaObjects() {
    bool doneAnimation = true;
    bool updated;
    if (game->nodesStateDirty) {
        auto f = game->formula;
        auto n2o = game->nid2obj;
        for (auto it=n2o.begin(); it!=n2o.end(); it++) {
            auto node_id = it->first;
            auto o = it->second;
            auto n = f->nodes[node_id];

            switch (n->type) {
                case NODE_TYPE_INPUT: {
                    if (n->isOn()) {
                        updated = o->incRotX(1, 45);
                        if (updated) doneAnimation = false;
                        o->setColor("0xffffff");
                    } else {
                        updated = o->decRotX(1, -45);
                        if (updated) doneAnimation = false;
                        o->setColor("0x111111");
                    }
                    break;
                }
                default: {
                    if (n->isOn()) {
                        updated = o->incY(5, BOX_Y);
                        if (updated) doneAnimation = false;
                        o->setColor("0xffffff");
                    } else {
                        updated = o->decY(5, -BOX_Y+20);
                        if (updated) doneAnimation = false;
                        o->setColor("0x111111");
                    }
                    break;
                }
            }
        }

        auto door = game->scene->churchDoor;
        auto rick = game->scene->rick;
        if (game->formula->root->isOn()) {
            // open door
            updated = door->incX(1, 170);
            // make rick disappear
            rick->setScalation(0, 0, 0);
            if (updated) doneAnimation = false;
        } else {
            // close door (fast)
            updated = door->decX(5, 0);
            if (updated) doneAnimation = false;
            // make rick appear
            if (!updated) {
                rick->setScalation(RICK_SCALE_X, RICK_SCALE_Y, RICK_SCALE_Z);
            }
        }

        if (doneAnimation) {
            game->nodesStateDirty = false;
        }
    }
}


void mainTimer(int) {
    glutTimerFunc(16,mainTimer,0);
    handleKeys();
    updateFormulaObjects();
    redisplayFunc();
}
