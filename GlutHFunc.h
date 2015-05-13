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
