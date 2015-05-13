#ifndef  GLUT_H_FUNC
#define GLUT_H_FUNC

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <assert.h>

#include <GL/glut.h>

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


#endif //GLUT_H_FUNC
