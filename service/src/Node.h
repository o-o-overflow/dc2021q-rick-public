#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <string>
#include <cstdio>
#include <sstream>

#include "Object.h"

using namespace std;

#define NODE_TYPE_INPUT 0
#define NODE_TYPE_NOT 1
#define NODE_TYPE_AND 2
#define NODE_TYPE_OR 3
#define NODE_TYPE_XOR 4
#define NODE_TYPE_NAND 5
#define NODE_TYPE_NOR 6
#define NODE_TYPE_ALT 7
#define NODE_TYPE_EXT0 8
#define NODE_TYPE_EXT1 9


class Formula;

class Node {
public:
    static int next_id;

    unsigned int id;
    int type;
    unsigned int depth;
    bool on_;
    // Object *o;
    Formula *f;

    vector<Node*> inputs;

    Node(int type, unsigned int depth, Formula *f) {
        this->id = Node::next_id++;
        this->type = type;
        this->depth = depth;
        this->on_ = false;
        this->f = f;
    }

    //
    // turnOn / turnOff: only on the input nodes. This invokes f.updateState
    // updateState: calls updateState on each input, then change the internal
    // state based on their states and their type. This method is invoked
    // repeatedly from outside on the root node. This method also sets up the object.
    // updateObject: takes current type+state and updates drawing.
    //

    void turnOn();

    void turnOff();

    bool isOn() {
        return on_;
    }

    void updateState() {
        // cout << "updateState() for node id " << id << endl;
        for (auto it=inputs.begin(); it!=inputs.end(); it++) {
            auto c = *it;
            c->updateState();
        }
        // cout << "finished to process inputs for node id " << id << ", proceeding" << endl;
        switch (type) {
            case NODE_TYPE_INPUT:
                break;
            case NODE_TYPE_NOT: {
                auto c = inputs[0];
                on_ = !(c->isOn());
                break;
            }
            case NODE_TYPE_AND: {
                on_ = true;
                for (auto it=inputs.begin(); it!=inputs.end(); it++) {
                    auto c = *it;
                    if (!(c->isOn())) {
                        on_ = false;
                        break;
                    }
                }
                break;
            }
            case NODE_TYPE_OR: {
                on_ = false;
                for (auto it=inputs.begin(); it!=inputs.end(); it++) {
                    auto c = *it;
                    if (c->isOn()) {
                        on_ = true;
                        break;
                    }
                }
                break;
            }
            case NODE_TYPE_XOR: {
                on_ = false;
                for (auto it=inputs.begin(); it!=inputs.end(); it++) {
                    auto c = *it;
                    if (c->isOn()) {
                        on_ = !on_;
                    }
                }
                break;
            }
            case NODE_TYPE_NAND: {
                on_ = true;
                for (auto it=inputs.begin(); it!=inputs.end(); it++) {
                    auto c = *it;
                    if (!(c->isOn())) {
                        on_ = false;
                        break;
                    }
                }
                on_ = !on_;
                break;
            }
            case NODE_TYPE_NOR: {
                on_ = false;
                for (auto it=inputs.begin(); it!=inputs.end(); it++) {
                    auto c = *it;
                    if (c->isOn()) {
                        on_ = true;
                        break;
                    }
                }
                on_ = !on_;
                break;
            }
            case NODE_TYPE_ALT: {
                auto it = inputs.begin();
                auto last_eval = (*it)->isOn();
                it++;
                on_ = true;
                for (; it!=inputs.end(); it++) {
                    auto c = *it;
                    auto curr_eval = c->isOn();
                    if (curr_eval == last_eval) {
                        on_ = false;
                        break;
                    }
                    last_eval = curr_eval;
                }
                break;
            }
            case NODE_TYPE_EXT0: {
                auto it = inputs.begin();
                auto first_eval = (*it)->isOn();
                if (first_eval) {
                    on_ = false;
                    break;
                }
                if (inputs.size() == 1) {
                    break;
                }
                auto it2 = inputs.rbegin();
                auto last_eval = (*it2)->isOn();
                if (last_eval) {
                    on_ = false;
                    break;
                }
                on_ = true;
                break;
            }
            case NODE_TYPE_EXT1: {
                auto it = inputs.begin();
                auto first_eval = (*it)->isOn();
                if (!first_eval) {
                    on_ = false;
                    break;
                }
                if (inputs.size() == 1) {
                    break;
                }
                auto it2 = inputs.rbegin();
                auto last_eval = (*it2)->isOn();
                if (!last_eval) {
                    on_ = false;
                    break;
                }
                on_ = true;
                break;
            }
            default: {
                cerr << "ERROR: updateState with type: " << type << endl;
                throw "error";
             } 
        }
    }

    void flipState();

#if DEBUG
    string toString() {
        ostringstream ss;
        ss << "Node: " << id << " type: " << type << " depth: " << depth << " state: " << on_;
        return ss.str();
    }
#endif

};


#endif
