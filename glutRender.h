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
#include "gui/Widget.h"
#include "gui/Edit.h"
#include "gui/EditFloat.h"
#include "gui/Button.h"

#include <GL/glut.h>

#ifndef GL_MULTISAMPLE
	#define GL_MULTISAMPLE  0x809D
#endif

#define ADDBUTTON(name,xcoord,ycoord,func) 																	\
	new_button = new Button(xcoord, ycoord, std::string(name));							\
	new_button->visible = false;																		\
    new_button->prevWidth = this->WindowWidth;                                                          \
	new_button->OnClick = func;																			\
	widgets.push_back(new_button);

#define ADDEDITFLOAT(label,xcoord,ycoord,pointer)                                                              \
    new_editfloat = new EditFloat(xcoord, ycoord, 300.0f, 55.0f,                          \
                                    std::string("0.0"), std::string(label));                            \
    new_editfloat->visible = false;                                                                     \
    new_editfloat->value = 0.0;                                                                         \
    new_editfloat->prevWidth = this->WindowWidth;                                                       \
    pointer = &new_editfloat->value;                                                                    \
    widgets.push_back(new_editfloat);

#define PUSHTOSTATE(state) states[state].push_back(widgets.size() - 1);

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
			void NewButtonOnClick (Widget * Sender);
			// button
			void ApplyButton (Widget * Sender);
			void AddBallButton (Widget * Sender);
			void BackButton (Widget * Sender);
			void ShowHideMenuButton (Widget * Sender);
			void ExitButton (Widget * Sender);
			void ResetButton (Widget * Sender);
			void StartStopButton (Widget * Sender);
			void NextBallButton (Widget * Sender);
			void PrevBallButton (Widget * Sender);
			void TracksDrawingButton(Widget *Sender);
			void ToggleMusicButton (Widget * Sender);
            void OpenBallEditor (Widget * Sender);
			void setFocus(Widget * widget);
			void unsetFocus();
			void setState(int num);
			void unsetState();
			void InitGui();

		private:
			std::vector<Widget *> widgets;
			std::vector< std::vector<unsigned long> > states;
			Widget * focusedWidget;
            int dragX, dragY;
            bool dragState = false;
            int menuState;

            glutRender (int _WindowWidth = 1280, int _WindowHeight = 768) :
				WindowWidth(_WindowWidth), WindowHeight(_WindowHeight) {};

			static void DisplayGL_ ();
			static void IdleGL_ ();
			static void KeyboardGL_ (unsigned char c, int x, int y);
			static void MouseGL_ (int button, int state, int x, int y);
			static void MotionGL_ (int x, int y);
			static void ReshapeGL_ (int w, int h);
			static void NewButtonOnClick_ (Widget * Sender);
			void setOrthographicProjection();
			void restorePerspectiveProjection();
            void LoadConfig(const std::string &table_config, const std::string &balls_config, const std::string &start_state_config);
			int glutWindowHandle;

			int WindowWidth;
			int WindowHeight;
			// pointer to EditFloat values
			float * xcoord;
			float * ycoord;
			float * zcoord;
			float * xvelocity;
			float * yvelocity;
			float * zvelocity;
			float * xanglevelo;
			float * yanglevelo;
			float * zanglevelo;
			// pointer to static buttons method
			static void ApplyButton_ (Widget * Sender);
			static void AddBallButton_ (Widget * Sender);
			static void BackButton_ (Widget * Sender);
			static void ShowHideMenuButton_ (Widget * Sender);
			static void ExitButton_ (Widget * Sender);
			static void ResetButton_ (Widget * Sender);
			static void StartStopButton_ (Widget * Sender);
			static void NextBallButton_ (Widget * Sender);
			static void PrevBallButton_ (Widget * Sender);
			static void TracksDrawingButton_(Widget *Sender);
			static void ToggleMusicButton_ (Widget * Sender);
            static void OpenBallEditor_ (Widget * Sender);
			//
			float alpha;
            float multipluer;
            float cam_height_h;
            int curre_ball;

            bool calculations_started = false;
            bool help_menu_showed = false;
            bool main_theme_state = true;
			bool draw_all_tracks = false;

			float3f free_camera_pos = float3f(0, 0, 0);

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
