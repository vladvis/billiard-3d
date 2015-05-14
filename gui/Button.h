//
// Created by vladvis on 14.05.15.
//

#ifndef BILLIARD_BUTTON_H
#define BILLIARD_BUTTON_H

#include "Widget.h"

class Button : public Widget {
public:
    std::string label;
    Button(float x, float y, std::string label, float w = 200.0f, float h = 45.0f);
    virtual void render();
};


#endif //BILLIARD_BUTTON_H
