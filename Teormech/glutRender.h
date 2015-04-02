#ifndef  GLUT_RENDER_H
#define GLUT_RENDER_H

#include <cmath>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <assert.h>

#ifdef LINUX_PLATFORM
	#include <GL/glut.h>
#else
	#include <glut.h>
#endif

class float3
{
		public:
			GLfloat x, y, z;
			float3 (GLfloat _x = 0, GLfloat _y = 0, GLfloat _z = 0) : 
				x(_x), y(_y), z(_z) {};
};

class float2
{
		public:
			GLfloat x, y;
			float2 (GLfloat _x = 0, GLfloat _y = 0) : x(_x), y(_y) {};
};

class Mouse
{
		public:
			int x, y;
			bool LeftKeyPressed;
			bool RightKeyPressed;
			Mouse (int _x = 0, int _y = 0, bool _LeftKeyPressed = false, bool _RightKeyPressed = false) :
				x(_x), y(_y), LeftKeyPressed(_LeftKeyPressed), RightKeyPressed(_RightKeyPressed) {};
};


class glutRender
{
		public:
			void Init (int* argc, char* argv[]);
			void DisplayGL ();
				
			void IdleGL ();
			void Cleanup ();
			void MouseGL (int button, int state, int x, int y);
			void MotionGL (int x, int y);
			void ReshapeGL (int w, int h);
			void KeyboardGL (unsigned char c, int x, int y);

			static glutRender Instance;

		private:
			glutRender (int _WindowWidth = 900, int _WindowHeight = 700) :
				WindowWidth(_WindowWidth), WindowHeight(_WindowHeight) {};

			static void DisplayGL_ ();
			static void IdleGL_ ();
			static void KeyboardGL_ (unsigned char c, int x, int y);
			static void MouseGL_ (int button, int state, int x, int y);
			static void MotionGL_ (int x, int y);
			static void ReshapeGL_ (int w, int h);

			int glutWindowHandle;

			int WindowWidth;
			int WindowHeight;

			Mouse MouseManipulator;
			
			float3 camera_position;
			GLfloat camera_distance;

			std::clock_t PreviousTicks;
};

void checkGLError (const char * errorLocation);

#endif