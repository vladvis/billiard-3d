//
// Created by vladvis on 13.05.15.
//

#include "Edit.h"

void Edit::receiveStroke(char c) {
    if (c == 8) {  // backspace
        if (this->cursor_pos == 0)
            return;
        std::string newtext = this->text.substr(0, this->cursor_pos-1) +
                              this->text.substr(this->cursor_pos, this->text.size()-this->cursor_pos);
        this->text = newtext;
        this->cursor_pos--;
    }
    if (this->text.size() >= EDITLENGTH-1)
        return;
    if (!is_char(c))
        return;
    std::string newtext = this->text.substr(0, this->cursor_pos) + c +
                          this->text.substr(this->cursor_pos, this->text.size()-this->cursor_pos);

    this->text = newtext;
    this->cursor_pos++;
}

Edit::Edit(float x, float y, float w, float h, std::string text) : Widget(x, y, w, h) {
    this->text = text;
    this->cursor_pos = text.size();
    this->setBackgroundColor(0.1f, 0.1f, 0.1f, 0.5f);
}

void Edit::setBackgroundColor(float r, float g, float b, float transparent) {
    this->bgColor[0] = r;
    this->bgColor[1] = g;
    this->bgColor[2] = b;
    this->bgColor[3] = transparent;
}

void Edit::render() {
    if (!this->visible)
        return;
    glPushMatrix();
    DrawRoundRect(this->x, this->y + this->h/2, this->w, this->h, this->bgColor, 0.0f);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.7f);
    glTranslatef(this->x+5.0f, this->y-5.0f, 0.0f);
    renderString(GLUT_STROKE_MONO_ROMAN, (char *)this->text.c_str());
    glPopMatrix();
    glPushMatrix();
    if (this->isFocused) {
        float xpos = 0.17f * 104.76f * (this->cursor_pos - 0.2f);
        glTranslatef(this->x + 5.0f + xpos, this->y - 5.0f, 0.0f);
        renderString(GLUT_STROKE_ROMAN, (char *) "|");
    }
    glPopMatrix();
}