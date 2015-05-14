//
// Created by vladvis on 13.05.15.
//

#include "EditFloat.h"

void EditFloat::receiveStroke(char c) {
    if (c == 8) {  // backspace
        if (this->cursor_pos == 0)
            return;
        std::string newtext = this->text.substr(0, this->cursor_pos-1) +
                              this->text.substr(this->cursor_pos, this->text.size()-this->cursor_pos);
        this->text = newtext;
        if (newtext.size() == 0)
            this->setBackgroundColor(0.6f, 0.1f, 0.1f, 0.5f);
        else
            this->setBackgroundColor(0.1f, 0.1f, 0.1f, 0.5f);
        this->cursor_pos--;
    }
    if (this->text.size() >= EDITLENGTH-1)
        return;
    if (!is_char(c))
        return;
    std::string newtext = this->text.substr(0, this->cursor_pos) + c +
                          this->text.substr(this->cursor_pos, this->text.size()-this->cursor_pos);
    if (!std::regex_match(newtext, std::regex(FLOAT_REGEXP)))
        return;
    else
        this->setBackgroundColor(0.1f, 0.1f, 0.1f, 0.5f);
    this->text = newtext;
    this->cursor_pos++;
}