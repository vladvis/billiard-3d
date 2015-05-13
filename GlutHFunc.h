#ifndef  GLUT_H_FUNC
#define GLUT_H_FUNC

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>
#include <string.h>
#include <math.h>
#include <regex>

#include <GL/glut.h>
#define EDITLENGTH 17
#define PI 3.14159265

// Storageruct of bitmap file.
struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
};

class Widget {
        public:
            float x, y, w, h;
            Widget(float x, float y, float w, float h, bool visible = true);
            virtual void receiveStroke(char c);
            virtual void render();
            bool visible;
            bool isFocused;
};

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

class EditFloat : public Edit {
        private:
            float value;

        public:
            EditFloat(float x, float y, float w = 300.0f, float h = 45.0f,
                      std::string text = std::string("0.0")) : Edit(x, y, w, h, text) {};
            virtual void receiveStroke(char c);
};

void checkGLError (const char * errorLocation);
GLuint raw_texture_load(const char *filename, int width, int height);
void renderStrokeFontString(float x, float y, float z, void *font, char *string);
void renderString (void *font, char *string);
bool is_char(char c);
void restorePerspectiveProjection();
void setOrthographicProjection(float w, float h);
void DrawRoundRect( float x, float y, float width, float height, float* color = NULL, float radius = 0.0f );
inline float min(float a, float b) { return a < b ? a : b; }


#endif //GLUT_H_FUNC
