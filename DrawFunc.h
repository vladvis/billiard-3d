#ifndef  DRAW_FUNC
#define DRAW_FUNC

#include <assert.h>
#include <iostream>
#include <math.h>
#include <pthread.h>

#include <GL/glut.h>



void init_l();
void DrawGroundGrid (const GLfloat groundLevel);
void DrawNiceRectangle(const GLfloat xleft, const GLfloat xright, const GLfloat yleft, const GLfloat yright, bool is_textured = false);
void DrawTableLeg (const GLfloat edge_size, const GLfloat height);
void DrawBilliardTable(const GLfloat hwidth, const GLfloat hheight, const GLfloat border_h, const GLfloat leg_height, const GLfloat ball_r, unsigned texture);
void DrawVerticalPLine();
void DrawCoolRoundAround(GLfloat ball_r, GLfloat angle);
void DrawGrid(GLfloat ball_r, GLfloat border_down_height);

#endif //DRAW_FUNC
