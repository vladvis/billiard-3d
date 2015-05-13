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