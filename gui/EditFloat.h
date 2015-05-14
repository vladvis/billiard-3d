//
// Created by vladvis on 13.05.15.
//

#ifndef BILLIARD_EDITFLOAT_H
#define BILLIARD_EDITFLOAT_H

#include "Widget.h"
#include "Edit.h"
#define FLOAT_REGEXP "\\-?\\d*\\.?\\d*"

class EditFloat : public Edit {
private:
    float value;

public:
    EditFloat(float x, float y, float w = 300.0f, float h = 45.0f,
              std::string text = std::string("0.0"), std::string label = "") : Edit(x, y, w, h, text, label) {};
    virtual void receiveStroke(char c);
};

#endif //BILLIARD_EDITFLOAT_H
