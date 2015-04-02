#ifndef  GLUT_RENDER_H
#define GLUT_RENDER_H

#include <cmath>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <assert.h>
#include <fstream>
#include <string>

#include "table.h"

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

class glutRender
{
		public:
		    static glutRender Instance;
		    Table GameTable;

			void Init (int* argc, char* argv[], const char *table_config, const char *balls_config);

			void DisplayGL ();
			void IdleGL ();
			void Cleanup ();
			void MouseGL (int button, int state, int x, int y);
			void MotionGL (int x, int y);
			void ReshapeGL (int w, int h);
			void KeyboardGL (unsigned char c, int x, int y);

		private:
			glutRender (int _WindowWidth = 900, int _WindowHeight = 700) :
				WindowWidth(_WindowWidth), WindowHeight(_WindowHeight) {};

			static void DisplayGL_ ();
			static void IdleGL_ ();
			static void KeyboardGL_ (unsigned char c, int x, int y);
			static void MouseGL_ (int button, int state, int x, int y);
			static void MotionGL_ (int x, int y);
			static void ReshapeGL_ (int w, int h);

            void LoadConfig(const std::string table_config, const std::string balls_config, const std::string start_state_config);
			int glutWindowHandle;

			int WindowWidth;
			int WindowHeight;

			double alpha;
            double multipluer;
            int curre_ball;

            bool calculations_started;

            std::string table_config_filename;
            std::string balls_config_filename;
            std::string start_state_config_filename;

			std::clock_t PreviousTicks;
};

void checkGLError (const char * errorLocation);

#endif
