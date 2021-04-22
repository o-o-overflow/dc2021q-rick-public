#ifndef _COLOR_H_
#define _COLOR_H_

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class Color {
    private:
        float red, green, blue;
        
        static int hexToInt (string s);
    public:
        vector <float> getColor3f();

        float getRed();
        float getGreen();
        float getBlue();
        void setComponents(string s);
        void setComponents(float r, float g, float b);

#if DEBUG
        string toString();
#endif
};

#endif
