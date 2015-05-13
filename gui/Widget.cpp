//
// Created by vladvis on 13.05.15.
//

#include "Widget.h"

Widget::Widget(float x, float y, float w, float h, bool visible) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->visible = visible;
    this->isFocused = false;
}

void Widget::receiveStroke(char c) {
}

void Widget::render() {
}

bool Widget::hasPoint(int x, int y) {
    return (((-round(this->y)-y) < this->h/2) &&
            ((x - round(this->x)) < this->w) &&
            ((x - round(this->x)) >= 0));
}
