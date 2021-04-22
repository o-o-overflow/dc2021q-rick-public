#include "State.h"

State::State() {}

void State::clear() {
    setMode(MODE_NORMAL);
}

Camera * State::getCamera() { return camera; }
void State::setCamera(Camera * camera) { this->camera = camera; }

int State::getMode() { return mode; }
void State::setMode(int mode) {
    this->mode = mode;
    switch (mode) {
    // it sets the new position, by looking at the new mode
    case MODE_NORMAL:
        camera->setPosition(DONTCARE,State::tyWALK,DONTCARE);
        camera->setRotation(State::axWALK,DONTCARE,State::azWALK);
        break;
    case MODE_FLY:
        camera->setRotation(DONTCARE,DONTCARE,State::azFLY);
    }
}
