#ifndef  GLUT_H_FUNC
#define GLUT_H_FUNC

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <assert.h>

#ifdef LINUX_PLATFORM
	#include <GL/glut.h>
#else
	#include <glut.h>
#endif

// Storageruct of bitmap file.
struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
};

void checkGLError (const char * errorLocation);
BitMapFile *getBMPData(const char *filename);
GLuint LoadTexture(const char *filename);
void renderBitmapString(float x, float y, float z, void *font, char *string);
GLuint raw_texture_load(const char *filename, int width, int height);
void setOrthographicProjection();
void renderStrokeFontString(
	float x,
	float y,
	float z,
	void *font,
	char *string);

#endif //GLUT_H_FUNC
