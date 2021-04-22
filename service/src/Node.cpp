#include "Node.h"
#include "Formula.h"

int Node::next_id = 1;

void Node::turnOn() { 
    if (type == NODE_TYPE_INPUT) {
        this->on_ = true;
    } else {
#if DEBUG
        cerr << "turnOn() is only valid for INPUT nodes. Node type is: " << type << endl;
#endif
        abort();
    }
    f->updateState();
}

void Node::turnOff() {
    if (type == NODE_TYPE_INPUT) {
        this->on_ = false;
    } else {
#if DEBUG
        cerr << "turnOff() is only valid for INPUT nodes. Node type is: " << type << endl;
#endif
        abort();
    }
    f->updateState();
}

void Node::flipState() {
    if (type == NODE_TYPE_INPUT) {
        if (isOn()) turnOff();
        else turnOn();
    } else {
#if DEBUG
        cerr << "flipSate() is only valid for INPUT nodes. Node type is: " << type << endl;
#endif
        abort();
    }
}
