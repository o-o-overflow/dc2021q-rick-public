#ifndef NAVIGATIONSTATE_H_
#define NAVIGATIONSTATE_H_

#include <algorithm>

#include "Camera.h"
#include "Formula.h"


#define MODE_NORMAL 0
#define MODE_FLY 1

class State {
public:

    Camera * camera;
    int mode;

    vector<int> pressedKeys;

    Formula *formula;

    static const int txRESET = -20;
    static const int tyRESET = 15;
    static const int tzRESET = 30;
    static const int axRESET = 0;
    static const int ayRESET = 200;
    static const int azRESET = 0;

    static const int txWALK = DONTCARE;
    static const int tyWALK = 40;
    static const int tzWALK = DONTCARE;
    static const int axWALK = 0;
    static const int ayWALK = DONTCARE;
    static const int azWALK = 0;

    static const int azFLY = 0;

    static const int NSTEP=20;


    State();

    void clear();

    void setFormula(Formula *formula) {
        this->formula = formula;
    }

    Camera * getCamera();
    void setCamera(Camera * camera);

    int getMode();
    void setMode(int mode);

    void pressKey(int key) {
        pressedKeys.push_back(key);
    }

    void pressSpecialKey(int key) {
        pressedKeys.push_back(0x80000000 | key);
    }

    void unpressKey(int key) {
        pressedKeys.erase(remove(pressedKeys.begin(), pressedKeys.end(), key), pressedKeys.end());
    }

    void unpressSpecialKey(int key) {
        pressedKeys.erase(remove(pressedKeys.begin(), pressedKeys.end(), 0x80000000 | key), pressedKeys.end());
    }

    bool isKeyPressed() {
        if (pressedKeys.size() > 0) {
            return true;
        } else {
            return false;
        }
    }

    bool withBoost() {
        auto it = find(pressedKeys.begin(), pressedKeys.end(), 0x20);
        if (it == pressedKeys.end()) return false;
        return true;
    }

    unsigned char getKey();
    void setKey(unsigned char key);

    int getSpecialKey();
    void setSpecialKey(int specialKey);


#if DEBUG
    string toString() {
        ostringstream ss;
        ss << "------ STATE ------\n";
        ss << "pressedKeys: ";
        for (auto it=pressedKeys.begin(); it!=pressedKeys.end(); it++) {
            ss << *it;
        }
        ss << endl;
        return ss.str();
    }
#endif

};

#endif
