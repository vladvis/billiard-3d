#include "glutRender.h"

// THIS IS  THE TEST

float g_fRotate1 = 0.0f, g_fRotate2 = 0.0f, g_fRotate3 = 0.0f; // TEST!

void RenderScene4()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	glTranslatef(-1.5f, 1.0f, -6.0f);                                     // Translate back and to the left

	glPushMatrix();                                                         // Push the current modelview matrix on the matrix stack
	glRotatef(g_fRotate1, 1.0f, 1.0f, 1.0f);                               // Rotate on all 3 axis
	glBegin(GL_TRIANGLES);                                                // Draw a pyramid        
	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex3f(0.0f, 1.0f, 0.0f);                                     // Top of front face
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);                                   // Left of front face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(1.0f, -1.0f, 1.0f);                                    // Right of front face

	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex3f(0.0f, 1.0f, 0.0f);                                     // Top of right face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(1.0f, -1.0f, 1.0f);                                    // Left of right face
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(1.0f, -1.0f, -1.0f);                                   // Right of right face

	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex3f(0.0f, 1.0f, 0.0f);                                     // Top of back face
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(1.0f, -1.0f, -1.0f);                                   // Left of back face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f);                                  // Right of back face

	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex3f(0.0f, 1.0f, 0.0f);                                     // Top of left face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f);                                  // Left of left face
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);                                   // Right of left face
	glEnd();

	// Render a quad for the bottom of our pyramid
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);                                   // Left/right of front/left face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(1.0f, -1.0f, 1.0f);                                    // Right/left of front/right face
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(1.0f, -1.0f, -1.0f);                                   // Right/left of right/back face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f);                                  // Left/right of right/back face
	glEnd();
	glPopMatrix();

	glTranslatef(3.0f, 0.0f, 0.0f);                                        // Translate right
	glPushMatrix();                                                         // Push the current modelview matrix on the matrix stack
	glRotatef(g_fRotate2, 1.0f, 1.0f, 1.0f);                              // Rotate the primitive on all 3 axis
	glBegin(GL_QUADS);
	// Top face
	glColor3f(0.0f, 1.0f, 0.0f);                                   // Green
	glVertex3f(1.0f, 1.0f, -1.0f);                                   // Top-right of top face
	glVertex3f(-1.0f, 1.0f, -1.0f);                                   // Top-left of top face
	glVertex3f(-1.0f, 1.0f, 1.0f);                                   // Bottom-left of top face
	glVertex3f(1.0f, 1.0f, 1.0f);                                   // Bottom-right of top face

	// Bottom face
	glColor3f(1.0f, 0.5f, 0.0f);                                  // Orange
	glVertex3f(1.0f, -1.0f, -1.0f);                                  // Top-right of bottom face
	glVertex3f(-1.0f, -1.0f, -1.0f);                                  // Top-left of bottom face
	glVertex3f(-1.0f, -1.0f, 1.0f);                                  // Bottom-left of bottom face
	glVertex3f(1.0f, -1.0f, 1.0f);                                  // Bottom-right of bottom face

	// Front face
	glColor3f(1.0f, 0.0f, 0.0f);                                  // Red
	glVertex3f(1.0f, 1.0f, 1.0f);                                  // Top-Right of front face
	glVertex3f(-1.0f, 1.0f, 1.0f);                                  // Top-left of front face
	glVertex3f(-1.0f, -1.0f, 1.0f);                                  // Bottom-left of front face
	glVertex3f(1.0f, -1.0f, 1.0f);                                  // Bottom-right of front face

	// Back face
	glColor3f(1.0f, 1.0f, 0.0f);                                 // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f);                                 // Bottom-Left of back face
	glVertex3f(-1.0f, -1.0f, -1.0f);                                 // Bottom-Right of back face
	glVertex3f(-1.0f, 1.0f, -1.0f);                                 // Top-Right of back face
	glVertex3f(1.0f, 1.0f, -1.0f);                                 // Top-Left of back face

	// Left face
	glColor3f(0.0f, 0.0f, 1.0f);                                   // Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);                                   // Top-Right of left face
	glVertex3f(-1.0f, 1.0f, -1.0f);                                   // Top-Left of left face
	glVertex3f(-1.0f, -1.0f, -1.0f);                                   // Bottom-Left of left face
	glVertex3f(-1.0f, -1.0f, 1.0f);                                   // Bottom-Right of left face

	// Right face
	glColor3f(1.0f, 0.0f, 1.0f);                                   // Violet
	glVertex3f(1.0f, 1.0f, 1.0f);                                   // Top-Right of left face
	glVertex3f(1.0f, 1.0f, -1.0f);                                   // Top-Left of left face
	glVertex3f(1.0f, -1.0f, -1.0f);                                   // Bottom-Left of left face
	glVertex3f(1.0f, -1.0f, 1.0f);                                   // Bottom-Right of left face
	glEnd();
	glPopMatrix();

	glTranslatef(-1.5f, -3.0f, 0.0f);                                     // Back to center and lower screen
	glPushMatrix();
	glRotatef(g_fRotate3, 1.0f, 1.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 0.0f);                                          // Yellow
	glutSolidSphere(1.0f, 16, 16);                                        // Use GLUT to draw a solid sphere
	glScalef(1.01f, 1.01f, 1.01f);
	glColor3f(1.0f, 0.0f, 0.0f);                                          // Red
	glutWireSphere(1.0f, 16, 16);                                         // Use GLUT to draw a wireframe sphere
	glPopMatrix();
}


/* there is only one copy of glutRender, so we are have to use this lifehack to deal with C GLUT library on C++, it is ok */
glutRender glutRender::Instance;

void glutRender::Init (int* argc, char* argv[])
{
	PreviousTicks = std::clock();

	glutInit (argc, argv);
	glutInitDisplayMode (GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	/*glutGameModeString ("1280x1024:32@60");
	glutEnterGameMode();
	glutDestroyWindow (currentWindow_);*/

	int ScreenWidth = glutGet (GLUT_SCREEN_WIDTH);
	int ScreenHeight = glutGet (GLUT_SCREEN_HEIGHT);

	glutInitWindowPosition ((ScreenWidth - WindowWidth) / 2, (ScreenHeight - WindowHeight) / 2);

	glutInitWindowSize (WindowWidth, WindowHeight);

	glutWindowHandle = glutCreateWindow ("Billiard 3D Project");

	glutDisplayFunc (DisplayGL_);
	glutIdleFunc (IdleGL_);
	glutMouseFunc (MouseGL_);
	glutMotionFunc (MotionGL_);
	glutKeyboardFunc (KeyboardGL_);
	glutReshapeFunc (ReshapeGL_);

	glClearColor (1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth (1.0f);

	glShadeModel (GL_SMOOTH);

	glutMainLoop ();
	Cleanup (); //something were wrong?
}

void glutRender::DisplayGL ()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	//TODO: call renders
	RenderScene4 ();

	glutSwapBuffers ();
	glutPostRedisplay ();
}

void glutRender::IdleGL ()
{
	//TODO: all calculations here
	std::clock_t CurrentTicks = std::clock ();
	float deltaTicks = (CurrentTicks - PreviousTicks);
	PreviousTicks = CurrentTicks;

	float fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC; // TODO: fps here

	g_fRotate1 += fDeltaTime * 30;
	g_fRotate2 += fDeltaTime * 35;
	g_fRotate3 += fDeltaTime * 40;

	glutPostRedisplay ();
}

void glutRender::MouseGL (int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
		{
			if (state == GLUT_UP)
				LeftMouseButtonPressed = false;
			else 
				LeftMouseButtonPressed = true;
		}
		break;
		
		case GLUT_RIGHT_BUTTON:
		{
			//TODO: make hit the cue here
		}
		break;
	}
}

void glutRender::MotionGL (int x, int y)
{
	if (LeftMouseButtonPressed)
	{
		//TODO: rotate camera across billard table
	}
	else
	{
		//TODO: rotate cue around ball
	}
}

void glutRender::KeyboardGL (unsigned char c, int x, int y)
{
	switch (c)
	{
		//TODO: the following is the test
		case 's':
		case 'S':
		{
			// Switch to smooth shading model
			glShadeModel (GL_SMOOTH);
		}
		break;
	
		case 'f':
		case 'F':
		{
			// Switch to flat shading model
			glShadeModel (GL_FLAT);
		}
		break;

		case '\009': // TAB
		{
			//TODO: Change active ball
		}
		break;
	
		case '\033': // escape quits
		{
			// Cleanup up and quit
			Cleanup();
			exit (0);
		}
		break;
	}

	glutPostRedisplay ();
}

void glutRender::ReshapeGL (int w, int h)
{
	if (h == 0) h = 1; // Prevent A Divide By Zero

	WindowWidth = w;
	WindowHeight = h;

	glViewport (0, 0, WindowWidth, WindowHeight);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60.0, (GLdouble)WindowWidth / (GLdouble)WindowHeight, 0.1, 100.0);

	glutPostRedisplay ();
}

void glutRender::Cleanup ()
{
	if (glutWindowHandle != 0)
	{
		glutDestroyWindow (glutWindowHandle);
		glutWindowHandle = 0;
	}
}


void glutRender::IdleGL_ ()
{
	glutRender::Instance.IdleGL ();
}

void glutRender::DisplayGL_ ()
{
	glutRender::Instance.DisplayGL ();
}

static void KeyboardGL_ (unsigned char c, int x, int y)
{
	glutRender::Instance.KeyboardGL (c, x, y);
}

static void MouseGL_ (int button, int state, int x, int y)
{
	glutRender::Instance.MouseGL (button, state, x, y);
}

static void MotionGL_ (int x, int y)
{
	glutRender::Instance.MotionGL (x, y);
}

static void ReshapeGL_ (int w, int h)
{
	glutRender::Instance.ReshapeGL (w, h);
}