#include "Color.h"

vector <float> Color::getColor3f() {
    vector <float> color;
    color.push_back(red);
    color.push_back(green);
    color.push_back(blue);
    return color;
}


float Color::getRed () { return red;}
float Color::getGreen () { return green;}
float Color::getBlue () { return blue;}


void Color::setComponents(string s) {
    string ss;
    if (s.length()==8) {
        s = s.substr(2,6);
        ss = s.substr(0,2);
        red = ((float)hexToInt(ss))/255.0;
        ss = s.substr(2,2);
        green = ((float)hexToInt(ss))/255.0;
        ss = s.substr(4,2);
        blue = ((float)hexToInt(ss))/255.0;
    } else if (s.length()==5) {
        s = s.substr(2,3);
        ss = s.substr(0,1);
        red = ((float)hexToInt(ss))/15.0;
        ss = s.substr(1,1);
        green = ((float)hexToInt(ss))/15.0;
        ss = s.substr(2,1);
        blue = ((float)hexToInt(ss))/15.0;
    }
}

void Color::setComponents(float r, float g, float b) {
    this->red = r;
    this->green = g;
    this->blue = b;
}

int Color::hexToInt (string s) {
    int m,n;

    if (s.length()==2) {
        if (s[0]>='0' && s[0] <='9') m = (s[0]-'0')*16;
        else if (s[0]>='A' && s[0]<='F') m = (s[0]-'A'+10)*16;
        else if (s[0]>='a' && s[0]<='f') m = (s[0]-'a'+10)*16;
        else return -1;

        if (s[1]>='0' && s[1]<='9') n = (s[1]-'0');
        else if (s[1]>='A' && s[1]<='F') n = (s[1]-'A'+10);
        else if (s[1]>='a' && s[1]<='f') n = (s[1]-'a'+10);
        else return -1;

        return n+m;
    } else if (s.length()==1) {
        if (s[0]>='0' && s[0] <='9') m = (s[0]-'0');
        else if (s[0]>='A' && s[0]<='F') m = (s[0]-'A'+10);
        else if (s[0]>='a' && s[0]<='f') m = (s[0]-'a'+10);
        else return -1;

        return m;
    }
    abort();
}


#if DEBUG
string Color::toString() {
    string s="";
    char color_s[20];
    sprintf(color_s,"%.2f %.2f %.2f",getRed(),getGreen(),getBlue());
    s += color_s;
    return s;
}
#endif
