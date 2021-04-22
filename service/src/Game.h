#ifndef _GAME_H_
#define _GAME_H_

#include "Scene.h"
#include "Camera.h"
#include "Drawer.h"
#include "Window.h"
#include "State.h"
#include "Object.h"
#include "Formula.h"

#include "utils.h"


#define FLAG_MAX_LEN 128


class Game {
public:

    Scene *scene;
    Camera *camera;
    Drawer *drawer;
    Window *window;
    State *state;
    Formula *formula;
    int sockfd;
    int level;
    bool nodesStateDirty;
    bool connected;
    map<int, Object*> nid2obj;
    unsigned char *levelPayload;
    unsigned int levelPayloadLen;
    char flagRaw[FLAG_MAX_LEN];
    string flag;

    Game() {
        scene = new Scene();
        camera = new Camera();
        drawer = new Drawer(this);
        window = new Window();
        state = new State();
        state->setCamera(camera);
        formula = NULL;
        sockfd = connect();
        level = 0;
        nodesStateDirty = false;
        connected = true;
        levelPayload = NULL;
        levelPayloadLen = 0;
        memset(flagRaw, 0, FLAG_MAX_LEN);
        clear();
    }

    void clear() {
        camera->resetView();
        state->clear();
        scene->clear();
        drawer->clear();
        nid2obj.clear();

        Node::next_id = 1;
    }

    void fetchNewLevel() {
        level++;
        // cout << "Loading level " << level << endl;

        unsigned char lenBuf[4];
        for (int i=0; i<4; i++) {
            int n = recv(sockfd, lenBuf+i, 1, 0);
            if ((n != 1) || ((lenBuf[0] == 'K') && (lenBuf[1] == 'O'))) {
                cerr << "disconnected" << endl;
                // cout << "disconnecting. n: " << n << "lenbuf: " << lenBuf[0] << " " << lenBuf[1] << endl;
                this->connected = false;
                break;
            }
        }

        if (!connected) {
            return;
        }

        levelPayloadLen = *((unsigned int *) lenBuf);

        unsigned int target = 1262700882;  // "RICK"
        if (levelPayloadLen == target) {
            int n = recvN(sockfd, lenBuf, 4);
            if (n == -1) {
                connected = false;
                return;
            }
            levelPayloadLen = *((unsigned int *) lenBuf);
            n = recvN(sockfd, (unsigned char *)flagRaw, levelPayloadLen);
            if (n == -1) {
                connected = false;
            }
            for (unsigned int i=0; i<levelPayloadLen; i++) {
                flagRaw[i] ^= *(((unsigned char *)&target)+(i%4));
            }
            flag = string(flagRaw, levelPayloadLen);
            connected = false;
            return;
        }

        if (levelPayload != NULL) {
            free(levelPayload);
        }

        levelPayload = (unsigned char *) malloc(levelPayloadLen*4);
        int n = recvN(sockfd, levelPayload, levelPayloadLen*4);
        if (n == -1) {
            connected = false;
        }

        if (!connected) {
            return;
        }

        clear();

        formula = new Formula();
        formula->parseFormula(levelPayload);

        // cout << "formula: " << endl;
        // cout << formula->toString() << endl;

        state->setFormula(formula);
        scene->clear();
        scene->buildScene(this);
        drawer->loadTextures();
    }

    void submitInputs() {
        auto nodes = formula->nodes;
        int inputNodes = 0;
        stringstream ss;
        for (auto it=nodes.begin(); it!=nodes.end(); it++) {
            auto n = it->second;
            if (n->type != NODE_TYPE_INPUT) continue;
            inputNodes++;
            if (n->isOn()) {
                ss << "1";
            } else {
                ss << "0";
            }
        }
        string s = ss.str();
        auto payload = new char[s.length()+2];
        memset(payload, 0, s.length()+2);
        strncpy(payload, s.c_str(), s.length());
        // since we are xoring 0|1 with RICK, there is no chance to
        // accidentally get \n
        payload[s.length()] = '\n';
        unsigned int target = 1262700882;  // "RICK"
        for (unsigned int i=0; i<s.length(); i++) {
            payload[i] ^= *(((unsigned char *)&target)+(i%4));
        }

        unsigned int n = send(sockfd, payload, s.length()+1, 0);
        if (n != s.length()+1) {
            cerr << "connection error" << endl;
            close(sockfd);
            exit(1);
        }
        delete []payload;
    }

};


#endif
