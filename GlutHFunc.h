#ifndef  GLUT_H_FUNC
#define GLUT_H_FUNC

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>
#include <string.h>

#include <GL/glut.h>
#define EDITLENGTH 20

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
};

class Edit : public Widget {
        private:
            std::string text;
            unsigned int cursor_pos;

        public:
            Edit(float x, float y, float w = 150.0f, float h = 45.0f, std::string text = std::string(""));
            virtual void render();
            virtual void receiveStroke(char c);
};

void checkGLError (const char * errorLocation);
GLuint raw_texture_load(const char *filename, int width, int height);
void renderStrokeFontString(float x, float y, float z, void *font, char *string);
void renderString (void *font, char *string);
<<<<<<< HEAD
bool is_char(char c);
void restorePerspectiveProjection();
void setOrthographicProjection(float w, float h);
=======
>>>>>>> 18f190f173ff6f97cf8053650293d6d11f40a4ca


#endif //GLUT_H_FUNC
