#include "Polygon.h"

Polygon::Polygon() {
    boundingBox_ = new BoundingBox();
    clear();
}

Polygon::~Polygon() {
    delete boundingBox_;
}


void Polygon::clear() {
    vertices.clear();
    normals.clear();
    textureCode = "";
    textureCoords_.clear();
    boundingBox_->clear();
}


void Polygon::addVertex(Point v) { vertices.push_back(v); }
vector<Point> Polygon::getVertices() {return vertices; }

void Polygon::setNormals(Vector v) {
    normals.push_back(v);
}
vector <Vector> Polygon::getNormals() { return normals; }

bool Polygon::areNormalsComputed() {
    if (normals.size()!=0) return true;
    else return false;
}


void Polygon::computeNormals() {

    Vector normal;
    Point p1 = vertices[0];
    Point p2 = vertices[1];
    Point p3 = vertices[2];

    normal.setCoord(Vector::findNormal(p1,p2,p3));

    for (unsigned int i=0; i<vertices.size();i++) {
        normals.push_back(normal);
    }
}


void Polygon::setColor(string s) { color.setComponents(s); }
Color Polygon::getColor() { return color; }

string Polygon::getTextureCode() { return textureCode; }
void Polygon::setTextureCode(string textureCode) { this->textureCode = textureCode; }

vector <Point> Polygon::getTextureCoords() { return textureCoords_; }
void Polygon::setTextureCoords(vector <Point> textureCoords) { textureCoords_ = textureCoords; }

bool Polygon::hasTextureCode() { return textureCoords_.size()!=0; }

BoundingBox* Polygon::getBoundingBox() { return boundingBox_; }
void Polygon::setBoundingBox(BoundingBox * boundingBox) { boundingBox_ = boundingBox; }

BoundingBox* Polygon::getEnlargedBoundingBox() { return boundingBox_->getEnlargedBoundingBox(); }

bool Polygon::isInBoundingBox(float cx, float cy, float cz) {
    return boundingBox_->isInside(cx, cy, cz);
}


#if DEBUG
string Polygon::toString() {
    string s="";
    Point v;
    char inum_c[20];
    string inum_s;
    Color c = getColor();
    
    s += "Color: " + c.toString() + "\n";
    for (unsigned int i=0;i<vertices.size();i++) {
        sprintf(inum_c,"%d",i);
        inum_s = inum_c;
        s += "vertex num " + inum_s + ": ";
        v = vertices[i];
        s += v.toString();
        s += "\n";
    }

    s += "Texture code: " + textureCode + "\n";
    for (auto it=textureCoords_.begin(); it!=textureCoords_.end(); it++) {
        s += (*it).toString() + "\n";
    }

    return s;
}
#endif
