//
// Created by vladvis on 13.05.15.
//

#ifndef BILLIARD_EDIT_H
#define BILLIARD_EDIT_H

#include "Widget.h"


class Edit : public Widget {
public:
    std::string text;
    unsigned int cursor_pos;
    GLfloat bgColor[4];
    Edit(float x, float y, float w = 300.0f, float h = 45.0f, std::string text = std::string(""));
    virtual void render();
    virtual void receiveStroke(char c);
    void setBackgroundColor(float r, float g, float b, float transparent);
};


#endif //BILLIARD_EDIT_H
