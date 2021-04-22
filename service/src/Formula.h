#ifndef _FORMULA_H_
#define _FORMULA_H_

#include <vector>
#include <string>
#include <cstdio>
#include <map>
#include <stack>

#include "Node.h"

#define TYPES_NUM 10
#define MAX_CHILDREN_NUM 256

using namespace std;

class Formula {
public:
    map <int, Node*> nodes;
    map <int, vector<Node*>*> nodesByDepth;
    Node *root = NULL;

    Node* createNode(int type, unsigned int depth) {
        Node* n = new Node(type, depth, this);
        nodes[n->id] = n;
        map<int, vector<Node*>*>::iterator iter = nodesByDepth.find(n->depth);
        if (iter == nodesByDepth.end()) {
            vector<Node*> *v = new vector<Node*>();
            nodesByDepth[n->depth] = v;
        }
        nodesByDepth[n->depth]->push_back(n);
        return n;
    }

    void updateState() {
        if (root == NULL) {
#if DEBUG
            cerr << "formula->updateStates() invoked with NULL root" << endl;
#endif
            abort();
        }
        root->updateState();
    }

#if DEBUG
    string toString() {
        ostringstream ss;
        ss << "Formula: (total # of nodes: " << nodes.size() << ")" << endl;
        if (root == NULL) {
            ss << "formula is empty" << endl;
        } else {
            for (int depth=0; ; depth++) {
                auto iter = nodesByDepth.find(depth);
                if (iter == nodesByDepth.end()) break;
                vector<Node*> *v = iter->second;
                for (vector<Node*>::iterator it=v->begin(); it!=v->end(); ++it) {
                    for (int i=0; i<depth; i++) {
                        ss << "    ";
                    }
                    ss << (*it)->toString() << endl;
                }
            }

            ss << endl << "sorted" << endl;
            stack<Node *> worklist;
            worklist.push(root);
            while (!worklist.empty()) {
                auto n = worklist.top();
                worklist.pop();
                for (unsigned int i=0; i<n->depth; i++) {
                    ss << "    ";
                }
                ss << n->toString() << endl;
                for (auto it=n->inputs.rbegin(); it!=n->inputs.rend(); ++it) {
                    worklist.push(*it);
                }
            }
            ss << "sorted done" << endl;
        }
        return ss.str();
    }
#endif

    void parseFormula(unsigned char *buf) {
        this->root = this->parseNode(buf, 0);
        this->updateState();
    }

    Node* parseNode(unsigned char *buf, int depth, int *skipPtr = NULL) {
        unsigned int *typePtr = (unsigned int *) buf;
        unsigned int nodeType = (*typePtr) % TYPES_NUM;
        int skip = 4;
        // cout << "creating node with: " << nodeType << " " << depth << endl;
        auto n = this->createNode(nodeType, depth);
        if (nodeType != NODE_TYPE_INPUT) {
            unsigned int *childrenNumPtr = (unsigned int *) (buf+skip);
            unsigned int childrenNum = (*childrenNumPtr) % MAX_CHILDREN_NUM;
            skip += 4;
            // cout << "this node has " << childrenNum << " children" << endl;
            for (unsigned int i=0; i<childrenNum; i++) {
                int newSkip;
                auto c = this->parseNode(buf+skip, depth+1, &newSkip);
                n->inputs.push_back(c);
                skip += newSkip;
            }
        }
        if (skipPtr != NULL) {
            *skipPtr = skip;
        }
        return n;
    }

};

#endif
