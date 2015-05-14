//
// Created by vladvis on 14.05.15.
//

#include "Button.h"

Button::Button(float x, float y, std::string label, float w, float h) : Widget(x, y, w, h) {
    this->label = label;
    this->setBackgroundColor(0.2f, 0.2f, 0.2f, 0.5f);
}

void Button::render() {
    if (!this->visible)
        return;
    glPushMatrix();
    float clr[4];
    for (int i = 0; i < 4; ++i) clr[i] = this->bgColor[i];
    float gradient = 0.03f;
    if (this->isFocused) for (int i = 0; i < 3; ++i) clr[i] += 0.05f;
    if (this->isPressed) gradient = -0.007f;
    DrawRoundRect(this->x, this->y + this->h/2, this->w, this->h, clr, 0.0f, gradient);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.7f);
    glTranslatef(this->x+5.0f, this->y-5.0f, 0.0f);
    renderString(GLUT_STROKE_ROMAN, (char *)this->label.c_str());
    glPopMatrix();
    glPushMatrix();
    glPopMatrix();
}
