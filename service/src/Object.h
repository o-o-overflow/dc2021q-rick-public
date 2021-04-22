#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <vector>
#include <string>
#include <sstream>
#include "Polygon.h"
#include "Transformer.h"

#include <GL/freeglut.h>

using namespace std;

class Object {
    private:
        
        string model_name;
        vector<Polygon*> polygons;
        
        // transformations that have to be applied
        float translation[3];
        float rotation[3];
        float scalation[3];
        
    public:
    
        Object();

        void clear();

        void setModelName(string model_name);
        string getModelName();

        void setColor(string color);
        
        inline void m3dCrossProduct(float *result, float u[], float v[3]);
        void m3dFindNormal(float *result, float point1[3], float point2[3],float point3[3]);
        void addPolygon(Polygon *polygon);
        vector<Polygon*> getPolygons();

        void computeBoundingBoxes();
        bool isInBoundingBox(float cx, float cy, float cz);
        bool isNear(float cx, float cy, float cz);

        void setTranslation(float x, float y, float z);
        vector<float> getTranslation();

        float getX();
        float getY();
        float getZ();
        void setX(float x);
        void setY(float y);
        void setZ(float z);

        bool incTranslation(int coordIdx, float step, float maxVal) {
            // returns true if we did an update
            bool updated = false;
            if (translation[coordIdx] < maxVal) {
                updated = true;
                translation[coordIdx] += step;
                if (translation[coordIdx] > maxVal) {
                    translation[coordIdx] = maxVal;
                }
            }
            computeBoundingBoxes();
            return updated;
        }

        bool decTranslation(int coordIdx, float step, float minVal) {
            // returns true if we did an update
            bool updated = false;
            if (translation[coordIdx] > minVal) {
                updated = true;
                translation[coordIdx] -= step;
                if (translation[coordIdx] < minVal) {
                    translation[coordIdx] = minVal;
                }
            }
            computeBoundingBoxes();
            return updated;
        }

        bool incX(float step, float maxVal) {
            return incTranslation(0, step, maxVal);
        }

        bool decX(float step, float minVal) {
            return decTranslation(0, step, minVal);
        }

        bool incY(float step, float maxVal) {
            return incTranslation(1, step, maxVal);
        }

        bool decY(float step, float minVal) {
            return decTranslation(1, step, minVal);
        }

        bool incZ(float step, float maxVal) {
            return incTranslation(2, step, maxVal);
        }

        bool decZ(float step, float minVal) {
            return decTranslation(2, step, minVal);
        }

        void setRotation(float x, float y, float z);
        vector<float> getRotation();
        float getRotX() { return rotation[0]; }
        float getRotY() { return rotation[1]; }
        float getRotZ() { return rotation[2]; }
        void setRotX(float x) { rotation[0] = x; }
        void setRotY(float y) { rotation[1] = y; }
        void setRotZ(float z) { rotation[2] = z; }

        bool incRotX(float step, float maxVal) {
            // returns true if we did an update
            bool updated = false;
            if (rotation[0] < maxVal) {
                updated = true;
                rotation[0] += step;
                if (rotation[0] > maxVal) {
                    rotation[0] = maxVal;
                }
            }
            return updated;
        }

        bool decRotX(float step, float minVal) {
            // returns true if we did an update
            bool updated = false;
            if (rotation[0] > minVal) {
                updated = true;
                rotation[0] -= step;
                if (rotation[0] < minVal) {
                    rotation[0] = minVal;
                }
            }
            return updated;
        }

        void setScalation(float x, float y, float z);
        vector<float> getScalation();
        
#if DEBUG
        string toString();
#endif
        
};

#endif
