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
#include <vector>

#include "table.h"
#include "DrawFunc.h"
#include "GlutHFunc.h"
#include "fpsMeasurer.h"
#include "sound.h"

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

class glutRender
{
		public:
		    static glutRender Instance;
		    Table GameTable;

			void Init (int* argc, char* argv[], const char *table_config, const char *balls_config, const char *sounds_config);

			void DisplayGL ();
			void IdleGL ();
			void Cleanup ();
			void MouseGL (int button, int state, int x, int y);
			void MotionGL (int x, int y);
			void ReshapeGL (int w, int h);
			void KeyboardGL (unsigned char c, int x, int y);
			void addBall();
			void setFocus(Widget * widget);
			void unsetFocus();

		private:
			std::vector<Widget *> widgets;
			Widget * focusedWidget;
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
            void LoadConfig(const std::string &table_config, const std::string &balls_config, const std::string &start_state_config);
			int glutWindowHandle;

			int WindowWidth;
			int WindowHeight;


			float alpha;
            float multipluer;
            float cam_height_h;
            int curre_ball;

            bool calculations_started = false;
            bool help_menu_showed = false;
            bool main_theme_state = true;
			bool draw_all_tracks = false;

			float3 free_camera_pos = float3(0, 0, 0);

            std::string table_config_filename;
            std::string balls_config_filename;
            std::string start_state_config_filename;

			std::clock_t PreviousTicks;

			remSnd SoundController;
			remSnd MainTheme;

			std::map<std::string, ALuint> MediaLibrary;
};

void checkGLError (const char * errorLocation);

#endif
