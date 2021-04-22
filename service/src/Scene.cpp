#include <bits/stdc++.h>

#include "Scene.h"
#include "Game.h"

#include "CustomObjects.h"


Scene::Scene() {
    nextLevelObject = NULL;
}

void Scene::clear() {
    for (auto it=objects.begin(); it!=objects.end(); it++) {
        auto o = *it;
        delete o;
    }
    objects.clear();
    for (auto it=objectsNodes.begin(); it!=objectsNodes.end(); it++) {
        auto o = *it;
        delete o;
    }
    objectsNodes.clear();
    objectsNodesByCoords.clear();
    for (auto it=lights.begin(); it!=lights.end(); it++) {
        auto l = *it;
        delete l;
    }
    lights.clear();
    nextLevelObject = NULL;
}

Color Scene::getBackgroundColor() { return background_color; }
void Scene::setBackgroundColor(Color color) { background_color = color; }

Color Scene::getAmbientColor() { return ambient_color; }

vector <Object *> Scene::getObjects() { return objects; }
vector <Object *> Scene::getObjectsNodes() { return objectsNodes; }
vector <Light *> Scene::getLights() { return lights; }

void Scene::addObject(Object * object) { objects.push_back(object); }
void Scene::addLight(Light * light) { lights.push_back(light); }

void Scene::addObjectNode(Object * object) { 
    objectsNodes.push_back(object);

    int x = ((int) (object->getX() / 1000)) * 1000;
    int z = ((int) (object->getZ() / 1000)) * 1000;

    auto c = make_pair(x, z);
    objectsNodesByCoords[c].push_back(object);
}


void Scene::addFormulaObjects(Game *g) {
    auto n2o = g->nid2obj;
    for (auto it=n2o.begin(); it!=n2o.end(); it++) {
        auto o = it->second;
        addObjectNode(o);
    }
}

void Scene::placeFormulaObjects(Game *g) {
    auto f = g->formula;
    int DELTA = 200;

    int prev_depth_x = -1;
    for (int depth=0; ; depth++) {
        if (depth == 0) {
            // root
            auto o = g->nid2obj[f->root->id];
            o->setTranslation(ROOT_X, BOX_Y, ROOT_Z);
            o->setRotation(0,0,0);
            o->setScalation(BOX_DEPTH, BOX_HEIGHT, BOX_WIDTH);
            o->computeBoundingBoxes();

            // cout << "adding root " << f->root->toString() << " @ (" << ROOT_X << " " << BOX_Y << " " << ROOT_Z << ")" << endl;

            prev_depth_x = ROOT_X;
        } else {
            auto iter = f->nodesByDepth.find(depth);
            if (iter == f->nodesByDepth.end()) break;

            auto v = iter->second;
            auto nodesNum = v->size();

            int x = prev_depth_x + BOX_DEPTH + DELTA + EXTRA_SPACE;
            prev_depth_x = x;
            int idx = 0;
            for (vector<Node*>::iterator it=v->begin(); it!=v->end(); ++idx, ++it) {
                int z = ((BOX_WIDTH + DELTA) * (2*idx - (nodesNum-1))) / 2;

                auto n = *it;
                auto o = g->nid2obj[n->id];
                switch (n->type) {
                    case NODE_TYPE_INPUT: {
                        o->setTranslation(x, LEVER_Y, z);
                        o->setRotation(-45,0,0);
                        o->setScalation(LEVER_DEPTH, LEVER_HEIGHT, LEVER_WIDTH);
                        o->computeBoundingBoxes();
                        break;
                    }
                    default: {
                        o->setTranslation(x, BOX_Y, z);
                        o->setRotation(0,0,0);
                        o->setScalation(BOX_DEPTH, BOX_HEIGHT, BOX_WIDTH);
                        o->computeBoundingBoxes();
                        break;
                     }
                }

                // cout << "adding node " << n->toString() << " @ (" << x << " " << BOX_Y << " " << z << ")" << endl;
            }
        }
    }
}


void Scene::createLights() {
    background_color.setComponents(0.0,0.18,0.6);

    ambient_color.setComponents(0,0,0);

    float lightPosition[3];
    lightPosition[X]=200;
    lightPosition[Y]=150;
    lightPosition[Z]=-200;

    Light *light = new Light(0);
    light->setAmbientColor(0.7,.7,.7);
    light->setDiffusionColor(0.7,0.7,.7);
    light->setPosition(lightPosition[X],lightPosition[Y],lightPosition[Z]);
    addLight(light);

}


bool Scene::buildScene(Game *g) {

    createLights();

    // cout << "building scene " << endl;

    auto f = g->formula;
    // go through the nodes and create objects
    for (auto it=f->nodes.begin(); it!=f->nodes.end(); it++) {
        auto node_id = it->first;

        // auto node = it->second;
        // cout << "node: " << node_id << " => " << node->toString() << endl;

        auto o = new BoxObject();
        g->nid2obj[node_id] = o;
    }

    nextLevelObject = new BoxObject();
    nextLevelObject->setTranslation(CHURCH_X, CHURCH_Y, CHURCH_Z-200);
    nextLevelObject->setRotation(-45,90,0);
    nextLevelObject->setScalation(LEVER_DEPTH, LEVER_HEIGHT, LEVER_WIDTH);
    nextLevelObject->setColor("0xcfaa41");  // gold
    nextLevelObject->computeBoundingBoxes();
    addObject(nextLevelObject);

    // place objects around according to the formula structure
    placeFormulaObjects(g);
    // add objects to what should be rendered
    addFormulaObjects(g);

    g->nodesStateDirty = true;
    

    // take care of the floor

    int min_x=INT_MAX, max_x=INT_MIN, min_z=INT_MAX, max_z=INT_MIN;
    auto n2o = g->nid2obj;
    for (auto it=n2o.begin(); it!=n2o.end(); it++) {
        auto o = it->second;
        auto t = o->getTranslation();
        int x = t[0];
        int z = t[2];

        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (z < min_z) min_z = z;
        if (z > max_z) max_z = z;
    }

    Object *object;

    // create floor
    object = new Object();
    Polygon *p;

    int floor_min_x = -1000, floor_max_x = max_x+500;
    int floor_min_z = min_z-1000, floor_max_z = max_z+2000;
    int floor_width = floor_max_z - floor_min_z;
    int floor_depth = floor_max_x - floor_min_x;

    p = new Polygon();
    Point vertex;
    vertex.clear();
    vertex.setCoord(floor_min_x, 0, floor_min_z);
    p->addVertex(vertex);
    vertex.clear();
    vertex.setCoord(floor_max_x, 0, floor_min_z);
    p->addVertex(vertex);
    vertex.clear();
    vertex.setCoord(floor_max_x, 0, floor_max_z);
    p->addVertex(vertex);
    vertex.clear();
    vertex.setCoord(floor_min_x, 0, floor_max_z);
    p->addVertex(vertex);

    if (!p->areNormalsComputed()) {
        p->computeNormals();
    }

    p->setTextureCode(TEXTURE_FLOOR);
    Point tc;
    vector<Point> tcs;
    tc.setCoord(0, 0, 0);
    tcs.push_back(tc);
    tc.setCoord(floor_depth/64, 0, 0);
    tcs.push_back(tc);
    tc.setCoord(floor_depth/64, floor_width/64, 0);
    tcs.push_back(tc);
    tc.setCoord(0, floor_width/64, 0);
    tcs.push_back(tc);
    p->setTextureCoords(tcs);

    object->addPolygon(p);

    object->setTranslation(0,-1,0);
    object->setRotation(0,0,0);
    object->setScalation(1,1,1);
    object->computeBoundingBoxes();
    object->setColor("0xffffff");
    addObject(object);

    // cout << "added floor manually: " << object->toString() << church;
    
    auto church = new ChurchObject();
    church->setTranslation(CHURCH_X, CHURCH_Y, CHURCH_Z);
    church->setScalation(10, 10, 10);
    church->setRotation(0, 270, 0);
    church->computeBoundingBoxes();
    addObject(church);

    churchDoor = new ChurchDoorObject();
    churchDoor->setTranslation(0, 0, CHURCH_Z-5);
    churchDoor->setScalation(10, 10, 10);
    churchDoor->setRotation(0, 270, 0);
    churchDoor->computeBoundingBoxes();
    addObject(churchDoor);

    rick = new RickObject();
    rick->setTranslation(CHURCH_X, CHURCH_Y, CHURCH_Z);
    rick->setScalation(RICK_SCALE_X, RICK_SCALE_Y, RICK_SCALE_Z);
    rick->setRotation(0, 270, 0);
    rick->computeBoundingBoxes();
    addObject(rick);

    return true;
}


bool Scene::checkForCollision(float cx, float cy, float cz) {
    for (auto it=objects.begin(); it!=objects.end(); it++) {
        auto o = *it;
        if (o->isInBoundingBox(cx, cy, cz)) {
            return true;
        }
    }

    int cx2 = ((int) (cx / 1000)) * 1000;
    int cz2 = ((int) (cz / 1000)) * 1000;
    int radius = 5000;
    for (int x=cx2-radius; x<=cx2+radius; x+=1000) {
        for (int z=cz2-radius; z<=cz2+radius; z+=1000) {
            auto c = make_pair(x, z);
            auto objects = objectsNodesByCoords[c];
            for (auto it=objects.begin(); it!=objects.end(); it++) {
                auto o = *it;
                auto vv = o->getTranslation();
                if (o->isInBoundingBox(cx, cy, cz)) {
                    return true;
                }
            }
        }
    }

    return false;
}


bool Scene::checkForCollision(Camera *camera) {
    float cx = camera->getTX();
    float cy = camera->getTY();
    float cz = camera->getTZ();

    return checkForCollision(cx, cy, cz);
}
