#ifndef  GLUT_RENDER_H
#define GLUT_RENDER_H

#include <math.h>
#include <iostream>
#include <ctime>
#include <assert.h>

#include <glut.h>

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
			glutRender (int _WindowWidth = 800, int _WindowHeight = 600) :
				WindowWidth(_WindowWidth), WindowHeight(_WindowHeight) {};

			static void DisplayGL_ ();
			static void IdleGL_ ();
			static void KeyboardGL_ (unsigned char c, int x, int y);
			static void MouseGL_ (int button, int state, int x, int y);
			static void MotionGL_ (int x, int y);
			static void ReshapeGL_ (int w, int h);

			int glutWindowHandle = 0;

			int WindowWidth;
			int WindowHeight;

			bool LeftMouseButtonPressed;

			std::clock_t PreviousTicks;
};

#endif