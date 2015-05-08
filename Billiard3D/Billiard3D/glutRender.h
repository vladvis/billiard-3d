#ifndef  GLUT_RENDER_H
#define GLUT_RENDER_H

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <cmath>
#include <iostream>
#include <ctime>
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "table.h"
#include "DrawFunc.h"
#include "GlutHFunc.h"
#include "fpsMeasurer.h"

#include <GL/glut.h>

#ifndef GL_MULTISAMPLE
	#define GL_MULTISAMPLE  0x809D
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
			void addBall();

		private:
			glutRender (int _WindowWidth = 1280, int _WindowHeight = 768) :
				WindowWidth(_WindowWidth), WindowHeight(_WindowHeight) {};

			static void DisplayGL_ ();
			static void IdleGL_ ();
			static void KeyboardGL_ (unsigned char c, int x, int y);
			static void MouseGL_ (int button, int state, int x, int y);
			static void MotionGL_ (int x, int y);
			static void ReshapeGL_ (int w, int h);
			void setOrthographicProjection();
			void restorePerspectiveProjection();
            void LoadConfig(const std::string table_config, const std::string balls_config, const std::string start_state_config);
			int glutWindowHandle;

			int WindowWidth;
			int WindowHeight;

			float alpha;
            float multipluer;
            float cam_height_h;
            int curre_ball;

            bool calculations_started;
            bool help_menu_showed;

            std::string table_config_filename;
            std::string balls_config_filename;
            std::string start_state_config_filename;

			std::clock_t PreviousTicks;
};

void checkGLError (const char * errorLocation);

#endif
