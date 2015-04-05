#ifndef  DRAW_FUNC
#define DRAW_FUNC

#include <assert.h>
#include <iostream>

#ifdef LINUX_PLATFORM
	#include <GL/glut.h>
#else
	#include <glut.h>
#endif



void init_l();
void DrawGroundGrid (const GLfloat groundLevel);
void DrawNiceRectangle(const GLfloat xleft, const GLfloat xright, const GLfloat yleft, const GLfloat yright);
void DrawTableLeg (const GLfloat edge_size, const GLfloat height);
void DrawBilliardTable(const GLfloat hwidth, const GLfloat hheight, const GLfloat border_h, const GLfloat leg_height, GLuint g_texture, GLuint tree_texture);

#endif //DRAW_FUNC
