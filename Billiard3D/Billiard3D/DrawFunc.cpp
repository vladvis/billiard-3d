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
	float ambient[] = {0.55f, 0.55f, 0.55f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
}


void DrawGroundGrid (const GLfloat groundLevel)
{
    GLfloat stepSize    = 2.0f;
    int numSteps = 20;

    glLineWidth(1.1f);
    glColor3ub(90, 90, 90);

    glBegin(GL_LINES);
    for (GLint x = -numSteps; x <= numSteps; x++)
        for (GLint y = -numSteps; y <= numSteps; y++)
        {
            GLfloat xx = x * stepSize;
            GLfloat yy = y * stepSize;

            glVertex3f(xx, groundLevel, yy);
            glVertex3f(xx, groundLevel, (y+1) * stepSize);

            glVertex3f(xx, groundLevel, yy);
            glVertex3f((x+1) * stepSize, groundLevel, yy);
        }
    glEnd();
}

void DrawNiceRectangle(const GLfloat xleft, const GLfloat xright, const GLfloat yleft, const GLfloat yright)
{
    assert (xright > xleft); assert (yright > yleft);

    const float grid_period = 30;

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
                glVertex3f(ir, 0, j);
                glVertex3f(i, 0, j);

                glNormal3f(0.0f, 		1.0f, 		0.0f);
                glVertex3f(ir, 0.0f, jr);
                glVertex3f(ir, 0.0f, j);
                glVertex3f(i, 0.0f, jr);
            glEnd();
        }
    }
}

void DrawTableLeg (const GLfloat edge_size, const GLfloat height)
{
	assert (height > 0); assert (edge_size > 0);

	const GLfloat  	hwidth = edge_size / 2,
					hheight = height / 2;
    const GLfloat   hhwidth = hwidth / 2;

	glPushMatrix();
        glTranslatef(hhwidth, 0, 0);
        glRotatef(-90, 0, 0, 1);
        DrawNiceRectangle(0, hheight, 0, hwidth);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-hhwidth, 0, 0);
        glRotatef(90, 0, 0, 1);
        DrawNiceRectangle(-hheight, 0, 0, hwidth);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-hhwidth, 0, hwidth);
        glRotatef(90, 1, 0, 0);
        DrawNiceRectangle(0, hwidth, 0, hheight);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-hhwidth, -hheight, 0);
        glRotatef(-90, 1, 0, 0);
        DrawNiceRectangle(0, hwidth, 0, hheight);
    glPopMatrix();
}

void DrawBilliardTable(const GLfloat hwidth, const GLfloat hheight, const GLfloat border_h, const GLfloat leg_height, GLuint g_texture, GLuint tree_texture)
{
	assert (hwidth > 0); assert (hheight > 0); assert (border_h > 0); assert (leg_height > 0);

	GLfloat fhborder_ = border_h / 1.2;

    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*if (g_texture > 0)
        {
            glEnable (GL_TEXTURE_2D);
            glBindTexture (GL_TEXTURE_2D, g_texture);
        } else */
		glColor3ub(0, 75, 0);

        DrawNiceRectangle(-hwidth, hwidth, -hheight, hheight);
        glDisable(GL_TEXTURE_2D);
    glPopAttrib();

    glColor3f(0.44f, 0.14f, 0.03f);

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

	GLfloat table_leg_edge = hwidth * hheight / 10.0f;
	GLfloat htable_leg_edge = table_leg_edge/2;

	glPushMatrix();
        glTranslatef(hwidth - htable_leg_edge, 0, hheight - htable_leg_edge);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();
	glPushMatrix();
        glTranslatef(hwidth - htable_leg_edge, 0, -hheight);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(-hwidth + htable_leg_edge, 0, hheight - htable_leg_edge);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(-hwidth + htable_leg_edge, 0, -hheight);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(-hwidth + htable_leg_edge, 0, -htable_leg_edge/2);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();

    glPushMatrix();
        glTranslatef(hwidth - htable_leg_edge, 0, -htable_leg_edge/2);
        DrawTableLeg(table_leg_edge, leg_height);
	glPopMatrix();
}
