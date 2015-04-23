#include "DrawFunc.h"

#define max(a,b) ((a>b) ? a : b)

void init_l()
{
    glEnable(GL_LIGHTING);

	GLfloat light0_diffuse[] = {0.6f, 0.6f, 0.6f};
    GLfloat light0_direction[] = {6.0f, 4.0f, 1.0f, 0.0f};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
	float ambient[] = {0.6f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
}


void DrawGroundGrid (const GLfloat groundLevel)
{
    glDisable(GL_LIGHTING);

    GLfloat stepSize    = 2.0f;
    int numSteps = 16;

    glLineWidth(0.1f);
    glColor3ub(76, 76, 76);

    glBegin(GL_LINES);
    for (GLint x = -numSteps; x <= numSteps; x++)
        for (GLint y = -numSteps; y <= numSteps; y++)
        {
            GLfloat xx = (float)x * stepSize;
            GLfloat yy = y * stepSize;

            glVertex3f(xx, groundLevel, yy);
            glVertex3f(xx, groundLevel, (y+1) * stepSize);

            glVertex3f(xx, groundLevel, yy);
            glVertex3f((x+1) * stepSize, groundLevel, yy);
        }
    glEnd();

    glEnable(GL_LIGHTING);
}

void DrawVerticalPLine()
{
    glDisable(GL_LIGHTING);

    GLfloat stepSize    = 0.03f;
    glLineWidth(1.0f);

    glColor3f (0.9f, 0.9f, 0.9f);

    glBegin(GL_LINES);
    for (GLfloat y = -0.28; y < 0.28; y += 2*stepSize)
    {
        glVertex3f (0, y, 0);
        glVertex3f (0, y + stepSize, 0);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void DrawCoolRoundAround(GLfloat ball_r, GLfloat angle_rad)
{
    glDisable(GL_LIGHTING);

    int amountSegments = 20;
    ball_r *= 1.5f;

    glLineWidth(0.4f);
    glColor3f (0.95f, 0.95f, 0.95f);

    glBegin(GL_LINES);
    for(int i = 0; i < amountSegments; i++)
    {
        float angle = angle_rad * float(i) / float(amountSegments);
        float dx = ball_r * cosf(angle);
        float dy = ball_r * sinf(angle);
        glVertex3f(dx, 0, dy);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void DrawRoundAround(GLfloat ball_r, GLfloat angle_rad)
{
    glDisable(GL_LIGHTING);

    int amountSegments = 20;
    ball_r *= 1.5f;


    glBegin(GL_LINE_STRIP);
    for(int i = 0; i <= amountSegments; i++)
    {
        float angle = angle_rad * float(i) / float(amountSegments);
        float dx = ball_r * cosf(angle);
        float dy = ball_r * sinf(angle);
        glVertex3f(dx, 0, dy);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void DrawGrid(GLfloat ball_r,  GLfloat border_down_height)
{
    glDisable(GL_LIGHTING);

    glLineWidth(1.6f);
    glColor3f (0.510f, 0.450f, 0.390f);

    int amountSegments = 9;

    GLfloat curre_ball_r = 2*ball_r;
    GLfloat height = 0;
    const GLfloat height_step = 0.05f;
    const GLfloat ball_r_step = 0.0085f;
    GLfloat angle_rad;

    while (curre_ball_r > ball_r)
    {
        if (height < border_down_height) angle_rad = 2*M_PI; else angle_rad = M_PI;

        glPushMatrix();
            glTranslatef(0, height, 0);

            glBegin(GL_LINE_STRIP);
            for(int i = 0; i <= amountSegments; i++)
            {
                float angle = angle_rad * float(i) / float(amountSegments);
                float dx = curre_ball_r * cosf(angle);
                float dy = curre_ball_r * sinf(angle);
                glVertex3f(dx, 0, dy);
            }
            glEnd();

            glBegin(GL_LINES);
            for(int i = 0; i <= amountSegments; i++)
            {
                float angle = angle_rad * float(i) / float(amountSegments);
                float dx = (curre_ball_r) * cosf(angle);
                float dy = (curre_ball_r) * sinf(angle);
                glVertex3f(dx, 0, dy);

                dx = (curre_ball_r-ball_r_step) * cosf(angle);
                dy = (curre_ball_r-ball_r_step) * sinf(angle);
                glVertex3f(dx, -height_step, dy);
            }
            glEnd();

        glPopMatrix();

        height -= height_step;
        curre_ball_r -= ball_r_step;
    }

    glEnable(GL_LIGHTING);
}

void DrawNiceRectangle(const GLfloat xleft, const GLfloat xright, const GLfloat yleft, const GLfloat yright)
{
    assert (xright > xleft); assert (yright > yleft);

    const float grid_period = 12;

    GLfloat step = max(xright - xleft, yright - yleft)/grid_period;

    for (GLfloat i = xleft; i < xright; i += step)
    {
        GLfloat ir;
        if (i + step > xright)
            ir = xright;
        else
            ir = i + step;

        for (GLfloat j = yleft; j < yright; j += step)
        {
            GLfloat jr;
            if (j + step > yright)
                jr = yright;
            else
                jr = j + step;

            glBegin(GL_TRIANGLES);
                glNormal3f(0.0f, 		1.0f, 		0.0f);
                glVertex3f(i, 0, jr);
                glNormal3f(0.0f, 		1.0f, 		0.0f);
                glVertex3f(ir, 0, j);
                glNormal3f(0.0f, 		1.0f, 		0.0f);
                glVertex3f(i, 0, j);

                glNormal3f(0.0f, 		1.0f, 		0.0f);
                glVertex3f(ir, 0.0f, jr);
                glNormal3f(0.0f, 		1.0f, 		0.0f);
                glVertex3f(ir, 0.0f, j);
                glNormal3f(0.0f, 		1.0f, 		0.0f);
                glVertex3f(i, 0.0f, jr);
            glEnd();
        }
    }
}

void DrawTableLeg (const GLfloat edge_size, const GLfloat height)
{
	assert (height > 0); assert (edge_size > 0);

    glColor3f(0.27f, 0.11f, 0.03f);

	const GLfloat  	hwidth = edge_size / 2;
    const GLfloat   hhwidth = hwidth / 2;

	glPushMatrix();
        glTranslatef(hhwidth, 0, 0);
        glRotatef(-90, 0, 0, 1);
        DrawNiceRectangle(0, height, 0, hwidth);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-hhwidth, 0, 0);
        glRotatef(90, 0, 0, 1);
        DrawNiceRectangle(-height, 0, 0, hwidth);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-hhwidth, 0, hwidth);
        glRotatef(90, 1, 0, 0);
        DrawNiceRectangle(0, hwidth, 0, height);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-hhwidth, -height, 0);
        glRotatef(-90, 1, 0, 0);
        DrawNiceRectangle(0, hwidth, 0, height);
    glPopMatrix();
}

void DrawBilliardTable(const GLfloat hwidth, const GLfloat hheight, const GLfloat border_h, const GLfloat leg_height, const GLfloat ball_r)
{
	GLfloat fhborder_ = border_h / 1.2;

    glColor3f(0.0f, 0.32f, 0.0f);

    DrawNiceRectangle(-hwidth, hwidth, -hheight, hheight);

	glColor3f(0.27f, 0.11f, 0.03f);

    glBegin(GL_QUADS);
        glVertex3f(hwidth - fhborder_, -fhborder_, hheight+fhborder_);
        glVertex3f(hwidth - fhborder_, -fhborder_, hheight);
        glVertex3f(hwidth - fhborder_, border_h, hheight);
        glVertex3f(hwidth - fhborder_, border_h, hheight+fhborder_);

        glVertex3f(-hwidth + fhborder_, border_h, hheight+fhborder_);
        glVertex3f(-hwidth + fhborder_, border_h, hheight);
        glVertex3f(-hwidth + fhborder_, -fhborder_, hheight);
        glVertex3f(-hwidth + fhborder_, -fhborder_, hheight+fhborder_);



        glVertex3f(hwidth - fhborder_, border_h, -hheight-fhborder_);
        glVertex3f(hwidth - fhborder_, border_h, -hheight);
        glVertex3f(hwidth - fhborder_, -fhborder_, -hheight);
        glVertex3f(hwidth - fhborder_, -fhborder_, -hheight-fhborder_);


        glVertex3f(-hwidth + fhborder_, -fhborder_, -hheight-fhborder_);
        glVertex3f(-hwidth + fhborder_, -fhborder_, -hheight);
        glVertex3f(-hwidth + fhborder_, border_h, -hheight);
        glVertex3f(-hwidth + fhborder_, border_h, -hheight-fhborder_);
    glEnd();

    glBegin(GL_QUADS);//here
        glVertex3f(hwidth+fhborder_, -fhborder_, hheight-fhborder_);
        glVertex3f(hwidth+fhborder_, border_h, hheight-fhborder_);
        glVertex3f(hwidth, border_h, hheight-fhborder_);
        glVertex3f(hwidth, -fhborder_, hheight-fhborder_);

        glVertex3f(hwidth+fhborder_, -fhborder_, -fhborder_);
        glVertex3f(hwidth+fhborder_, border_h, -fhborder_);
        glVertex3f(hwidth, border_h, -fhborder_);
        glVertex3f(hwidth, -fhborder_, -fhborder_);

        glVertex3f(hwidth, -fhborder_, -hheight+fhborder_);
        glVertex3f(hwidth, border_h, -hheight+fhborder_);
        glVertex3f(hwidth+fhborder_, border_h, -hheight+fhborder_);
        glVertex3f(hwidth+fhborder_, -fhborder_, -hheight+fhborder_);

        glVertex3f(hwidth, -fhborder_, fhborder_);
        glVertex3f(hwidth, border_h, fhborder_);
        glVertex3f(hwidth+fhborder_, border_h, fhborder_);
        glVertex3f(hwidth+fhborder_, -fhborder_, fhborder_);


        glVertex3f(-hwidth, -fhborder_, hheight-fhborder_);
        glVertex3f(-hwidth, border_h, hheight-fhborder_);
        glVertex3f(-hwidth-fhborder_, border_h, hheight-fhborder_);
        glVertex3f(-hwidth-fhborder_, -fhborder_, hheight-fhborder_);

        glVertex3f(-hwidth, -fhborder_, -fhborder_);
        glVertex3f(-hwidth, border_h, -fhborder_);
        glVertex3f(-hwidth-fhborder_, border_h, -fhborder_);
        glVertex3f(-hwidth-fhborder_, -fhborder_, -fhborder_);

        glVertex3f(-hwidth-fhborder_, -fhborder_, -hheight+fhborder_);
        glVertex3f(-hwidth-fhborder_, border_h, -hheight+fhborder_);
        glVertex3f(-hwidth, border_h, -hheight+fhborder_);
        glVertex3f(-hwidth, -fhborder_, -hheight+fhborder_);

        glVertex3f(-hwidth-fhborder_, -fhborder_, fhborder_);
        glVertex3f(-hwidth-fhborder_, border_h, fhborder_);
        glVertex3f(-hwidth, border_h, fhborder_);
        glVertex3f(-hwidth, -fhborder_, fhborder_);
    glEnd();

    glPushMatrix();
        glTranslatef(hwidth, border_h, 0);
        DrawNiceRectangle(0, fhborder_, fhborder_, hheight - fhborder_);
        DrawNiceRectangle(0, fhborder_, -hheight + fhborder_, -fhborder_);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(hwidth, 0, 0);
        glRotatef(90, 0, 0, 1);
        DrawNiceRectangle(0, border_h, fhborder_, hheight - fhborder_);
        DrawNiceRectangle(0, border_h, -hheight + fhborder_, -fhborder_);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(hwidth + fhborder_, border_h, 0);
        glRotatef(-90, 0, 0, 1);
        DrawNiceRectangle(0, border_h + fhborder_, fhborder_, hheight - fhborder_);
        DrawNiceRectangle(0, border_h + fhborder_, -hheight + fhborder_, -fhborder_);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-hwidth - fhborder_, border_h, 0);
        DrawNiceRectangle(0, fhborder_, fhborder_, hheight - fhborder_);
        DrawNiceRectangle(0, fhborder_, -hheight + fhborder_, -fhborder_);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-hwidth, 0, 0);
        glRotatef(-90, 0, 0, 1);
        DrawNiceRectangle(-border_h, 0, fhborder_, hheight - fhborder_);
        DrawNiceRectangle(-border_h, 0, -hheight + fhborder_, -fhborder_);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-hwidth - fhborder_, border_h, 0);
        glRotatef(90, 0, 0, 1);
        DrawNiceRectangle(-border_h - fhborder_, 0, fhborder_, hheight - fhborder_);
        DrawNiceRectangle(-border_h - fhborder_, 0, -hheight + fhborder_, -fhborder_);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, border_h, hheight);
        DrawNiceRectangle(-hwidth + fhborder_, hwidth - fhborder_, 0, fhborder_);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 0, hheight);
        glRotatef(-90, 1, 0, 0);
        DrawNiceRectangle(-hwidth + fhborder_, hwidth - fhborder_, 0, border_h);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, border_h, hheight + fhborder_);
        glRotatef(90, 1, 0, 0);
        DrawNiceRectangle(-hwidth + fhborder_, hwidth - fhborder_, 0, border_h + fhborder_);
    glPopMatrix();


    glPushMatrix();
        glTranslatef(0, border_h, -hheight - fhborder_);
        DrawNiceRectangle(-hwidth + fhborder_, hwidth - fhborder_, 0, fhborder_);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, border_h, -hheight);
        glRotatef(90, 1, 0, 0);
        DrawNiceRectangle(-hwidth + fhborder_, hwidth - fhborder_, 0, border_h);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, -fhborder_, -hheight - fhborder_);
        glRotatef(-90, 1, 0, 0);
        DrawNiceRectangle(-hwidth + fhborder_, hwidth - fhborder_, 0, border_h + fhborder_);
    glPopMatrix();

	GLfloat table_leg_edge = hwidth * hheight / 8.0f;
	GLfloat htable_leg_edge = table_leg_edge/2;

    /* leg 1 + basket 1 */
	glPushMatrix();
        glTranslatef(hwidth - htable_leg_edge, 0, hheight - htable_leg_edge);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();
	glPushMatrix();
        glTranslatef(hwidth - 0.005f, border_h - 0.005f, hheight - 0.005f);
        glRotatef(45, 0, 1, 0);
        DrawGrid(ball_r, -border_h);
	glPopMatrix();

    /* leg 2 + basket 2 */
	glPushMatrix();
        glTranslatef(hwidth - htable_leg_edge, 0, -hheight);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();
	glPushMatrix();
        glTranslatef(hwidth - 0.005f, border_h - 0.005f, -hheight + 0.005f);
        glRotatef(135, 0, 1, 0);
        DrawGrid(ball_r, -border_h);
	glPopMatrix();

    /* leg 3 + basket 3 */
	glPushMatrix();
        glTranslatef(-hwidth + htable_leg_edge, 0, hheight - htable_leg_edge);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();
	glPushMatrix();
        glTranslatef(-hwidth + 0.005f, border_h - 0.005f, hheight - 0.005f);
        glRotatef(-45, 0, 1, 0);
        DrawGrid(ball_r, -border_h);
	glPopMatrix();

    /* leg 4 + basket 4 */
	glPushMatrix();
        glTranslatef(-hwidth + htable_leg_edge, 0, -hheight);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();
	glPushMatrix();
        glTranslatef(-hwidth + 0.005f, border_h - 0.005f, -hheight + 0.005f);
        glRotatef(-135, 0, 1, 0);
        DrawGrid(ball_r, -border_h);
	glPopMatrix();

    /* leg 5 + basket 5 */
	glPushMatrix();
        glTranslatef(-hwidth + htable_leg_edge, 0, -htable_leg_edge/2);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();
	glPushMatrix();
        glTranslatef(-hwidth - 0.015f, border_h - 0.01f, 0);
        glRotatef(-90, 0, 1, 0);
        DrawGrid(ball_r, -border_h);
	glPopMatrix();

    /* leg 6 + basket 6 */
    glPushMatrix();
        glTranslatef(hwidth - htable_leg_edge, 0, -htable_leg_edge/2);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();
	glPushMatrix();
        glTranslatef(hwidth + 0.015f, border_h  - 0.01f, 0);
        glRotatef(90, 0, 1, 0);
        DrawGrid(ball_r, -border_h);
	glPopMatrix();
}
