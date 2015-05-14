//
// Created by vladvis on 13.05.15.
//

#ifndef BILLIARD_EDIT_H
#define BILLIARD_EDIT_H

#include "Widget.h"
#define EDITLENGTH 17

class Edit : public Widget {
public:
    std::string text;
    std::string label;
    unsigned long cursor_pos;
    Edit(float x, float y, float w = 300.0f, float h = 55.0f,
         std::string text = std::string(""), std::string label = std::string(""));
    virtual void render();
    virtual void receiveStroke(char c);
};


#endif //BILLIARD_EDIT_H
