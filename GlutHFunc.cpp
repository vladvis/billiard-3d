#include "GlutHFunc.h"

void checkGLError (const char *errorLocation)
{
    assert (errorLocation);

    unsigned int gle = glGetError();

    if (gle != GL_NO_ERROR)
    {
        std::cout << "GL Error discovered from caller " << errorLocation << ": ";

        switch (gle)
        {
        case GL_INVALID_ENUM:
            std::cout << "Invalid enum." << std::endl;
            break;

        case GL_INVALID_VALUE:
            std::cout << "Invalid value.\n";
            break;

        case GL_INVALID_OPERATION:
            std::cout << "Invalid Operation.\n";
            break;

        case GL_STACK_OVERFLOW:
            std::cout << "Stack overflow.\n";
            break;

        case GL_STACK_UNDERFLOW:
            std::cout << "Stack underflow.\n";
            break;

        case GL_OUT_OF_MEMORY:
            std::cout << "Out of memory.\n";
            break;
        default:
            std::cout << "Unknown error! Enum code is: " << gle << std::endl;
            break;
        }

    }
}

 GLuint raw_texture_load(const char *filename, int width, int height)
 {
     GLuint texture;
     unsigned char *data;
     FILE *file;

     // open texture data
     file = fopen(filename, "rb");
     if (file == NULL) return 0;

     // allocate buffer
     data = (unsigned char*) malloc(width * height * 4);

     // read texture data
     int res = fread(data, width * height * 4, 1, file);
     if (res < 0) assert ("Bad input file!");
     fclose(file);

     // allocate a texture name
     glGenTextures(1, &texture);

     // select our current texture
     glBindTexture(GL_TEXTURE_2D, texture);

     // select modulate to mix texture with color for shading
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

     // when texture area is small, bilinear filter the closest mipmap
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
     // when texture area is large, bilinear filter the first mipmap
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     // texture should tile
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

     // build our texture mipmaps
     gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

     // free buffer
     free(data);

     return texture;
 }

void renderString (void *font, char *string)
{
	char *c;
	glScalef(0.17f, 0.15f, 0.0f);
	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
}

void renderStrokeFontString (float x, float y, float z, void *font, char *string)
{
	char *c;
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.001f, 0.001f, 0.001f);
	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}

Edit::Edit(float x, float y, float w, float h, std::string text) : Widget(x, y, w, h) {
    this->text = std::string(text);
    std::cout << "DEBUG OUTPUT: " << w << "\n";
    this->cursor_pos = 0;
}

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

bool is_char(char c) {
    return c >= 32 && c < 127;
}

Widget::Widget(float x, float y, float w, float h, bool visible) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->visible = visible;
}

void Widget::receiveStroke(char c) {
}

void Widget::render() {
}

void Edit::render() {
    glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
        glRectf(this->x, this->y, this->x + this->w, this->y - this->h);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.8f, 0.8f, 0.7f);
        glTranslatef(this->x+5.0f, this->y-5.0f, 0.0f);
        renderString(GLUT_STROKE_MONO_ROMAN, (char *)this->text.c_str());
    glPopMatrix();
    glPushMatrix();
        float xpos = 0.17f*104.76f*(this->cursor_pos-0.2f);
        glTranslatef(this->x+5.0f+xpos, this->y-5.0f, 0.0f);
        renderString(GLUT_STROKE_ROMAN, (char *)"|");
    glPopMatrix();
}
