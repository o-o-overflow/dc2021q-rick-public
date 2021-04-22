#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <vector>
#include <string>
#include <cstdio>
#include "Point.h"
#include "Color.h"
#include "BoundingBox.h"
#include "Vector.h"

using namespace std;

class Polygon {
    private:
        vector <Point> vertices;
        Color color;
        vector <Vector> normals;

        string textureCode;
        vector <Point> textureCoords_;

        BoundingBox* boundingBox_;
    public:
        Polygon();
        ~Polygon();

        void clear();

        void addVertex(Point v);
        vector<Point> getVertices();

        void setNormals(Vector v);

        vector <Vector> getNormals();

        bool areNormalsComputed();

        void computeNormals();

        void setColor(string s);
        Color getColor();

        string getTextureCode();
        void setTextureCode(string textureCode);

        vector <Point> getTextureCoords();
        void setTextureCoords(vector <Point> textureCoords);
        bool hasTextureCode();

        BoundingBox* getBoundingBox();
        void setBoundingBox(BoundingBox* boundingBox);
        bool isInBoundingBox(float cx, float cy, float cz);
        BoundingBox* getEnlargedBoundingBox();

#if DEBUG
        string toString ();
#endif
};

#endif
