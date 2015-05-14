//
// Created by vladvis on 13.05.15.
//

#ifndef BILLIARD_WIDGET_H
#define BILLIARD_WIDGET_H


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>
#include <string.h>
#include <math.h>
#include <regex>
#include "../GlutHFunc.h"

#include <GL/glut.h>
#define EDITLENGTH 17
#define PI 3.14159265

class Widget {
public:
    float x, y, w, h;
    Widget(float x, float y, float w, float h, bool visible = true);
    virtual void receiveStroke(char c);
    virtual void render();
    virtual void setBackgroundColor(float r, float g, float b, float transparent);
    bool hasPoint(int x, int y);
    bool visible;
    bool isFocused;
    bool isPressed;
    float bgColor[4];
};

#endif //BILLIARD_WIDGET_H
