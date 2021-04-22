#include "Object.h"


Object::Object() {
    translation[0]=translation[1]=translation[2]=0;
    rotation[0]=rotation[1]=rotation[2]=0;
    scalation[0]=scalation[1]=scalation[2]=1;
}


void Object::clear() {

}


void Object::setModelName(string model_name) { this->model_name = model_name; }
string Object::getModelName() { return model_name; }



void Object::addPolygon(Polygon *polygon) {
    polygons.push_back(polygon);
}
vector<Polygon*> Object::getPolygons() { return polygons; }


void Object::computeBoundingBoxes() {
    BoundingBox* boundingBox;
    vector <Point> vertices;
    Point vertex;
    vector <Point> transformatedVertices;
    Transformer transformer;
    float matrix[16] = {1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,0,1
    };

    glPushMatrix();
    glLoadIdentity();
    transformer.translate(getTranslation());
    transformer.rotate(getRotation());
    transformer.scale(getScalation());

    for (unsigned int i=0;i<polygons.size();i++) {
        boundingBox = polygons[i]->getBoundingBox();
        transformatedVertices.clear();
        vertices = polygons[i]->getVertices();
        int vertices_number = vertices.size();


        for (int j=0; j<vertices_number; j++) {
            glPushMatrix();
            vertex = vertices[j];
            matrix[12] = vertex.coord[X];
            matrix[13] = vertex.coord[Y];
            matrix[14] = vertex.coord[Z];

            glMultMatrixf(matrix);
            glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
            vertex.coord[X] = matrix[12];
            vertex.coord[Y] = matrix[13];
            vertex.coord[Z] = matrix[14];

            transformatedVertices.push_back(vertex);
            glPopMatrix();
        }

        boundingBox->computeBoundingBox(transformatedVertices);
        polygons[i]->setBoundingBox(boundingBox);
    }
    glPopMatrix();
}


bool Object::isInBoundingBox(float cx, float cy, float cz) {
    auto polys = getPolygons();
    for (auto it=polys.begin(); it!=polys.end(); it++) {
        auto p = *it;
        if (p->isInBoundingBox(cx, cy, cz)) {
            return true;
        }
    }
    return false;
}


bool Object::isNear(float cx, float cy, float cz) {
    auto polys = getPolygons();
    for (auto it=polys.begin(); it!=polys.end(); it++) {
        auto p = *it;
        auto bb = p->getEnlargedBoundingBox();
        if (bb->isInside(cx, cy, cz)) {
            return true;
        }
    }
    return false;
}


void Object::setTranslation(float x, float y, float z) {
    translation[0]=x; translation[1]=y; translation[2]=z;
}

vector<float> Object::getTranslation() {
    vector <float> float_v;
    float_v.push_back(translation[0]);
    float_v.push_back(translation[1]);
    float_v.push_back(translation[2]);
    return float_v;
}

float Object::getX() { return translation[0]; }
float Object::getY() { return translation[1]; }
float Object::getZ() { return translation[2]; }
void Object::setX(float x) { translation[0] = x; }
void Object::setY(float y) { translation[1] = y; }
void Object::setZ(float z) { translation[2] = z; }

void Object::setRotation(float x, float y, float z) {
    rotation[0]=x; rotation[1]=y; rotation[2]=z;
}
vector<float> Object::getRotation() {
    vector <float> float_v;
    float_v.push_back(rotation[0]);
    float_v.push_back(rotation[1]);
    float_v.push_back(rotation[2]);
    return float_v;
}

void Object::setScalation(float x, float y, float z) {
    scalation[0]=x; scalation[1]=y; scalation[2]=z;
}
vector<float> Object::getScalation() {
    vector <float> float_v;
    float_v.push_back(scalation[0]);
    float_v.push_back(scalation[1]);
    float_v.push_back(scalation[2]);
    return float_v;
}

void Object::setColor(string color) {
    for (auto it=polygons.begin(); it!=polygons.end(); it++) {
        auto p = *it;
        p->setColor(color);
    }
}


#if DEBUG
string Object::toString() {
    ostringstream ss;
    ss << "------ Object ------\n";
    for (auto it=polygons.begin(); it!=polygons.end(); it++) {
        auto p = *it;
        ss << p->toString() << endl;
    }
    return ss.str();
}
#endif
