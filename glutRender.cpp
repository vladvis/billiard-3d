#include "glutRender.h"
#define min(a,b) (a<b ? a : b)

GLUquadricObj *sphere = NULL;

const int hel_info_lines_number = 16;
char help_info[hel_info_lines_number][70] =
                        {"[ q/Q/Esc ] - exit",
                        "[ Tab ] - switch between objects",
                        "[ c/C ] / [5] - set view on table center",
                        "[ Spacebar ] - begin/pause computational experiment",
                        "[ r/R ] - reset scene",
                        "[ w/W ] / [ s/S ] - zoom in/out camera from the active object",
                        "[ a/A ] / [ d/D ] - rotate the camera left/right on the active object",
                        "[ p/P ] / [ l/L ] - disable/enable the surfaces drawing",
                         "[ ]/] ] - decrease/increase time rate",
                         "[ t/T ] - show/hide ALL tracks",
                        "[ m/M ] - turn off the music",
                         "[ - ] / [ + ] - raise/lower the camera relative to the table",
                         "[ 4 ] / [ 6 ] - move the camera along the X-axis",
                         "[ 8 ] / [ 2 ] - move the camera along the Y-axis",
                         "",
                        "> Designed by Kopyrin Denis, Shcherbatov Kirill, Vladas Bulavas"
                        };

/* there is only one copy of glutRender, so we are have to use this lifehack to deal with C GLUT library on C++, it is ok */
glutRender glutRender::Instance;

void glutRender::Init (int* argc, char* argv[], const char *table_config, const char *balls_config, const char *sounds_config)
{
    srand(time(0));
    this->focusedWidget = NULL;
    multipluer = 2.5f;
    alpha = 1.8*3.14/3;
    curre_ball = 0;
    cam_height_h = 1.4;
/*    menuState = 0;
    // state 1 begin
    Button * exit = new Button(WindowWidth - 45.0f, -45.0f, std::string("X"), 35.0f, 35.0f);
    exit->visible = false;
    widgets.push_back(exit);
    Button * addnew = new Button(WindowWidth - 185.0f, -90.0f, std::string("Add new ball"));
    addnew->visible = false;
    widgets.push_back(addnew);
    Button * editcur = new Button(WindowWidth - 185.0f, -135.0f, std::string("Edit cur ball"));
    editcur->visible = false;
    widgets.push_back(editcur);
    // state 1 end
    // state 2 begin
    Button * back = new Button(WindowWidth - 45.0f, -135.0f, std::string("->"), 35.0f, 35.0f);
    back->visible = false;
    widgets.push_back(back);
    EditFloat * xcoord = new EditFloat(WindowWidth - 310.0f, -100.0f, 300.0f, 55.0f,
                                       std::string("0.0"), std::string("x coordinate"));
    xcoord->visible = false;
    widgets.push_back(xcoord);
    EditFloat * ycoord = new EditFloat(WindowWidth - 310.0f, -155.0f, 300.0f, 55.0f,
                                       std::string("0.0"), std::string("y coordinate"));
    ycoord->visible = false;
    widgets.push_back(ycoord);
    EditFloat * zcoord = new EditFloat(WindowWidth - 310.0f, -210.0f, 300.0f, 55.0f,
                                       std::string("0.0"), std::string("z coordinate"));
    zcoord->visible = false;
    widgets.push_back(zcoord);
    // state 2 end
*/
    if (*argc > 1)
        start_state_config_filename = std::string(argv[1]);
    else
        start_state_config_filename = std::string("lunev");

    InitializeOpenAL();

    glutInit (argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    int ScreenWidth = glutGet (GLUT_SCREEN_WIDTH);
    int ScreenHeight = glutGet (GLUT_SCREEN_HEIGHT);

    glutInitWindowPosition ((ScreenWidth - WindowWidth) / 2, (ScreenHeight - WindowHeight) / 2);

    glutInitWindowSize (WindowWidth, WindowHeight);

    glutWindowHandle = glutCreateWindow ("Billiard 3D Project 2015");
    assert (glutWindowHandle != 0);


    FILE *sounds_file = fopen(sounds_config, "r");
	assert(sounds_file);

	char sound_title[255], sound_path[255];
	while (fscanf(sounds_file, "%s %s", sound_title, sound_path) != EOF)
	{
	    if (MainTheme.mSourceID == 0)
        {
            MainTheme.Open(sound_path, true, false);
            continue;
        }

		unsigned sound_id = SoundController.Open(sound_path, false, false);
		assert (sound_id > 0);

		MediaLibrary.insert(std::pair<std::string, ALuint>(std::string(sound_title), sound_id));

	}
	fclose(sounds_file);

    glutDisplayFunc (DisplayGL_);
    glutIdleFunc (IdleGL_);
    glutKeyboardFunc (KeyboardGL_);
    glutMouseFunc(MouseGL_);
    glutReshapeFunc (ReshapeGL_);
    glutMotionFunc(MotionGL_);

    glEnable (GL_DEPTH_TEST);

    glClearColor(0.03f, 0.03f, 0.03f, 1.0f); // Set out clear colour to black, full alpha
    glClearDepth(1.0f);  // Clear the entire depth of the depth buffer

    glShadeModel (GL_SMOOTH); // Enable (gouraud) shading

    glDepthFunc(GL_LEQUAL); // Set our depth function to overwrite if new value less than or equal to current value
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Ask for nicest perspective correction
    glEnable(GL_CULL_FACE); // Do not draw polygons facing away from us

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); //TODO: GL_LIGHT_MODEL_LOCAL_VIEWER

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
    glEnable(GL_AUTO_NORMAL);
    glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glEnable( GL_BLEND );
    /* fog */
    glEnable(GL_FOG);
    GLfloat fogColor[4] = {0.03f, 0.03f, 0.03f, 1.0f};
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 15);
    glFogf(GL_FOG_END, 35);
    glFogfv(GL_FOG_COLOR, fogColor);
    /* fog end */

    checkGLError ("glutRender::Init");

    table_config_filename = std::string(table_config);
    balls_config_filename = std::string(balls_config);
    LoadConfig (table_config_filename, balls_config_filename, start_state_config_filename);
    PreviousTicks = std::clock();

    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

    MainTheme.Play(0.5f);

    glutMainLoop ();

    Cleanup(); //guess, is never calling
}


void glutRender::LoadConfig(const std::string &table_config, const std::string &balls_config, const std::string &start_state_config)
{
    calculations_started = false;

    float oldMINTIME = GameTable.MINTIME;
    float oldCLOCK = GameTable.DEFCLOCK;

    GameTable = Table(table_config);

    std::ifstream file(start_state_config.c_str());
    float rx, ry, rz, vx, vy, vz, wx, wy, wz;
    if (!file.is_open())
    {
        std::cout << "Failed to open file!";
        return;
    }

    if (!(file >> GameTable.SLOWFACTOR)){
        std::cout << "Failed to read slowfactor!";
        return;
    }


    char texture_filename[255];
    float3f track_color;

    int i = 0;

    while(file >> rx >> ry >> rz >> vx >> vy >> vz >> wx >> wy >> wz >> texture_filename >> track_color.x >> track_color.y >> track_color.z)
    {
        float phi = (rand() % 100) / 50.0 * M_PI;
        track_color.x /= 255; track_color.y /= 255; track_color.z /= 255;
        glutRender::GameTable.balls.push_back(Ball(balls_config, vec(rx, ry, rz), quat(cos(phi), sin(phi) * vec(rand(), rand(), rand()).normalized()), vec(vx, vy, vz), vec(wx, wy, wz), texture_filename, track_color));
    }

    if (!file.eof()){
        std::cout << "Incorrect file format!";
        return;
    }

    if (GameTable.balls.empty())
    {
        std::cout << "Table don't contain any ball!";
        return;
    }

    if (oldCLOCK == 0) {
        fpsMeasurer f(GameTable);
        f.fpsMeasure();
    }else{
        GameTable.DEFCLOCK = oldCLOCK;
        GameTable.MINTIME = oldMINTIME;
    }

    GameTable.sc_b_num = 0;

    std::cout << GameTable.MINTIME << " "<< GameTable.DEFCLOCK << std::endl;
}

void glutRender::DisplayGL ()
{
    const GLfloat groundLevel = -2.4f;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(1);
    glDisable (GL_LIGHTING);
    /* text out */
    {
        setOrthographicProjection();
        glPushMatrix();
            glLoadIdentity();
            glScalef(1.0f, -1.0f, 1.0f);
            glColor3f(1, 1, 1);

            char info[250];
            for (std::vector<Widget *>::iterator it = widgets.begin(); it != widgets.end(); ++it ) {
                (*it)->render();
            }
            if (curre_ball < (int)GameTable.balls.size())
            {
                sprintf(info, "BALL[%d] state: r(%+0.2f, %+0.2f, %+0.2f); v(%+0.2f, %+0.2f, %+0.2f); w(%+0.2f, %+0.2f, %+0.2f)", curre_ball+1,
                        GameTable.balls[curre_ball].r.x, GameTable.balls[curre_ball].r.y, GameTable.balls[curre_ball].r.z,
                        GameTable.balls[curre_ball].v.x,GameTable.balls[curre_ball].v.y, GameTable.balls[curre_ball].v.z,
                        GameTable.balls[curre_ball].w.x, GameTable.balls[curre_ball].w.y, GameTable.balls[curre_ball].w.z);

                glPushMatrix();
                    glTranslatef(15.0f, -WindowHeight + 15, 0.0f);
                    renderString(GLUT_STROKE_ROMAN, info);
                glPopMatrix();
            }

            glPushMatrix();
                glTranslatef(WindowWidth - 260.0f, -65.0f, 0.0f);
                sprintf(info, "Scores    %d", GameTable.sc_b_num);
                renderString(GLUT_STROKE_ROMAN, info);
            glPopMatrix();

            glPushMatrix();
               glTranslatef(WindowWidth - 260.0f, -30.0f, 0.0f);
               sprintf(info, "Time rate  %.5f", GameTable.MULT * 100);
               renderString(GLUT_STROKE_ROMAN, info);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(15.0f, -30.0f, 0.0f);
                renderString(GLUT_STROKE_ROMAN, (char *)"Press \'h\' to show/hide help information");
            glPopMatrix();


            if (help_menu_showed) {
                glPushMatrix();
                glTranslatef(35.0f, -65.0f, 0.0f);
                float height = 0;
                for (int i = 0; i < hel_info_lines_number; i++) {
                    glPushMatrix();
                    glTranslatef(0.0f, -height, 0.0f);
                    renderString(GLUT_STROKE_ROMAN, help_info[i]);
                    glPopMatrix();
                    height += 35;
                }
                glPopMatrix();
            }
        glPopMatrix();
        restorePerspectiveProjection();
    }

    glLoadIdentity();
    if (curre_ball < (int)GameTable.balls.size())
        gluLookAt (GameTable.balls[curre_ball].r.y + multipluer*sin(alpha), GameTable.balls[curre_ball].a + cam_height_h*log(multipluer),GameTable.balls[curre_ball].r.x + multipluer*cos(alpha),
                   GameTable.balls[curre_ball].r.y, GameTable.balls[curre_ball].r.z, GameTable.balls[curre_ball].r.x,
                   0.0f, 1.0f, 0.0f);
    else
        gluLookAt (free_camera_pos.y + multipluer*sin(alpha), free_camera_pos.z + cam_height_h*log(multipluer), free_camera_pos.x + multipluer*cos(alpha),
                   free_camera_pos.y, free_camera_pos.z, free_camera_pos.x,
                   0.0f, 1.0f, 0.0f);

    const GLfloat ball_r = GameTable.balls[0].a;

    glDisable(GL_LIGHTING);
    DrawGroundGrid (groundLevel);

    glDisable(GL_LIGHTING);
    if (curre_ball < (int)GameTable.balls.size())
    {
        Ball &ActiveBall = GameTable.balls[curre_ball];

        if (ActiveBall.isvalid == 0)
        {
            if (!GameTable.balls.empty())
            {
                SoundController.Play(MediaLibrary["choose"]);
                int length = GameTable.balls.size();
                int i = 0;
                for (i = 0; i < length; i++) {
                    curre_ball = (curre_ball + 1) % GameTable.balls.size();
                    if (GameTable.balls.at(curre_ball).isvalid) break;
                }
                if (i == length) curre_ball = length;
            }
            else {
                curre_ball = 0;
                SoundController.Play(MediaLibrary["wrong"]);
            }

            return;
        }

        if (!draw_all_tracks)
        {
            glPushMatrix();
                glColor3f (ActiveBall.track_color.x, ActiveBall.track_color.y, ActiveBall.track_color.z);
                glLineWidth(2.5f);

                glBegin(GL_LINES);
                    for (vec &r: ActiveBall.track) {
                        glVertex3f(r.y, r.z, r.x);
                    }
                glEnd();
            glPopMatrix();
        }

        glPushMatrix();
            glTranslatef(ActiveBall.r.y, ball_r + ActiveBall.r.z, ActiveBall.r.x);
            glRotatef(360*acos(ActiveBall.phi.l)/M_PI, ActiveBall.phi.v.y, ActiveBall.phi.v.z, ActiveBall.phi.v.x);
            DrawVerticalPLine();
        glPopMatrix();

        glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(ActiveBall.r.y, 0.001f, ActiveBall.r.x);
            DrawCoolRoundAround(ball_r, 2*M_PI);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
            glTranslatef(free_camera_pos.y, 0.001f, free_camera_pos.x);
            glColor3f(0.7f, 0.7f, 0.7f);
            glBegin(GL_LINES);
                glVertex3f (0.05, 0, 0);
                glVertex3f (-0.05, 0, 0);
                glVertex3f (0, 0, 0.05);
                glVertex3f (0, 0, -0.05);
            glEnd();
        glPopMatrix();
    }
    glDisable(GL_LIGHTING);

    init_l();
    glEnable(GL_LIGHTING);

	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		DrawBilliardTable (GameTable.lenx, GameTable.leny,  GameTable.border_height, 1.2*abs(groundLevel), ball_r, GameTable.texture);
	glPopMatrix();


    glColor3f (0.75f, 0.75f, 0.75f);
    for(std::vector<Ball>::iterator it = GameTable.balls.begin(); it != GameTable.balls.end(); ++it)
    {
        glDisable(GL_LIGHTING);
        if (draw_all_tracks)
        {
            glPushMatrix();
                glColor3f (it->track_color.x, it->track_color.y, it->track_color.z);
                glLineWidth(2.5f);

                glBegin(GL_LINES);
                    for (vec &r: it->track)
                    {
                        glVertex3f(r.y, r.z, r.x);
                    }
                glEnd();
            glPopMatrix();
        }

        if (it->isvalid != 0) {
            glEnable (GL_LIGHTING);
            glPushMatrix ();
                glEnable (GL_TEXTURE_2D);
                glColor3f (1, 1, 1);
                glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
                glBindTexture (GL_TEXTURE_2D, it->texture);

                glTranslatef (it->r.y, ball_r + it->r.z, it->r.x);
                glRotatef (360 * acos (it->phi.l) / M_PI, it->phi.v.y, it->phi.v.z, it->phi.v.x);
                gluSphere (sphere, ball_r, 20, 20);

                glDisable (GL_TEXTURE_2D);
            glPopMatrix ();
        }
    }

    glLineWidth(2.5f);
    //glDisable(GL_LIGHTING);

	glPushMatrix();
        glColor3f(0.9f, 0.9f, 0.9f);
        renderStrokeFontString(0.49f, -0.03f, GameTable.lenx + 0.09005f, GLUT_STROKE_ROMAN, (char *)"Billiard 3D PROJECT 2015");
	glPopMatrix();

	glutSwapBuffers();

    glFlush();
}

void glutRender::IdleGL ()
{
    if (calculations_started)
    {
        int ret = 0;
        sndvolume snd;
		for (int i = 0; i < GameTable.DEFCLOCK * GameTable.MULT; i++)
		{
			if (!(ret |= GameTable.NextStep(snd))) calculations_started = false;
		}

        for (Ball &b: GameTable.balls){
            b.track.push_back(b.r);
        }

        if (ret & 256)
            SoundController.Play(MediaLibrary["collide"], min(snd.collide/10, 1));
        if (ret & 512)
            SoundController.Play(MediaLibrary["pocket"]);
        if (ret & 1024)
            SoundController.Play(MediaLibrary["border_hit"], min(snd.board/5, 1));
        if (ret & 16)
            SoundController.Play(MediaLibrary["border_hit"], min(snd.table/5, 1));
    }
	glutPostRedisplay();
}


void glutRender::setOrthographicProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, WindowHeight, 0);
	glMatrixMode(GL_MODELVIEW);
}


void glutRender::restorePerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void glutRender::KeyboardGL (unsigned char c, int x, int y)
{
    if (this->focusedWidget != NULL) {
        if (c == '\t') {
            int i = 0;
            while (this->focusedWidget != this->widgets[i]) ++i;
            setFocus(this->widgets[(i+1) % this->widgets.size()]);
        }
        if (c == '\033') {
            this->unsetFocus();
            return;
        }
        this->focusedWidget->receiveStroke(c);
        return;
    }

    assert(&x);
    assert(&y);

    switch (c)
    {
        case 'h':
        case 'H':
        {
            SoundController.Play(MediaLibrary["wrong"]);
            help_menu_showed = !help_menu_showed;
        }
        break;


        case '\033': // escape quits
        case 'Q':
        case 'q':
        {
            Cleanup();
        }
        break;

        case  'D':
        case  'd':
        case '9':
        {
            alpha += 0.2f;
            if (alpha >= 2*M_PI) alpha -= 2*M_PI;
        }
        break;

        case 'L':
        case 'l':
        {
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        }
        break;

        case 'P':
        case 'p':
        {
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL) ;
        }
        break;

        case  'A':
        case  'a':
        case '7':
        {
            alpha -= 0.2f;
            if (alpha <= 0) alpha += 2*M_PI;
        }
        break;

        case 'S':
        case 's':
        {
            if (multipluer < 6.0) multipluer += 0.2f;
        }
        break;

        case 'W':
        case 'w':
        {
            if (multipluer > 1.2) multipluer -= 0.2f;
        }
        break;

        case '\t': //TAB
        {
            if (!GameTable.balls.empty())
            {
                SoundController.Play(MediaLibrary["choose"]);
                curre_ball = (curre_ball + 1) % GameTable.balls.size();
            }
            else {
                curre_ball = 0;
                SoundController.Play(MediaLibrary["wrong"]);
            }
        }
        break;

        case 'R':
        case 'r':
        {
            SoundController.Play(MediaLibrary["wrong"]);
            LoadConfig (table_config_filename, balls_config_filename, start_state_config_filename);
        }
        break;

        case ' ':
        {
            if (!calculations_started)
                SoundController.Play(MediaLibrary["hit"]);
            else
                SoundController.Play(MediaLibrary["wrong"]);

            calculations_started = !calculations_started;
        }
        break;

        case '+':
        {
            if (cam_height_h < 5) cam_height_h += 0.2f;
        }
        break;

        case '-':
        {
            if (cam_height_h > 1) cam_height_h -= 0.2f;
        }
        break;

        case '5':
        case 'c':
        case 'C':
        {
            SoundController.Play(MediaLibrary["choose"]);
            curre_ball = GameTable.balls.size();
        }
        break;

        case 'm':
        case 'M':
        {
            if (main_theme_state) MainTheme.Stop();
            main_theme_state != main_theme_state;
        }
        break;

        case 't':
        case 'T':
        {
            draw_all_tracks = !draw_all_tracks;
            SoundController.Play(MediaLibrary["choose"]);
        }
        break;

        case '}':
        case ']': {
            if (GameTable.MULT < 4) if (GameTable.MULT < 1) {
                GameTable.MULT *= 2;
            } else {
                if (GameTable.MULT < 3)
                GameTable.MULT += 0.05f;
            }
        }
        break;

        case '{':
        case '[':
        {
            if (GameTable.MULT <= 1) {
                if (GameTable.MULT > 0.0001)
                    GameTable.MULT /= 2;
            }
            else
                GameTable.MULT -= 0.05;
        }
        break;

        case 'x':
        case 'X':
        {
            EditFloat *edit = new EditFloat(10.0f, -100.0f);
            edit->label = "It's label";
            this->setFocus(edit);
            widgets.push_back (edit);
            Button *button = new Button(10.0f, -300.0f, std::string("I'm button!"));
            widgets.push_back(button);
        }
        break;

        case '4':
        {
            if (free_camera_pos.x + 0.2f <= GameTable.lenx)
                free_camera_pos.x += 0.1f;
        }
        break;

        case '6':
        {
            if (free_camera_pos.x - 0.2f >= -GameTable.lenx)
                free_camera_pos.x -= 0.1f;
        }
        break;

        case '8':
        {
            if (free_camera_pos.y - 0.2f >= -GameTable.leny)
                free_camera_pos.y -= 0.1f;
        }
        break;

        case '2':
        {
            if (free_camera_pos.y + 0.2f <= GameTable.leny)
                free_camera_pos.y += 0.1f;
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
    gluPerspective (60.0, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay ();
}

void glutRender::Cleanup ()
{
    DestroyOpenAL();

    if (glutWindowHandle != 0)
    {
        glutDestroyWindow (glutWindowHandle);
        glutWindowHandle = 0;
    }

    exit(0);
}

void glutRender::IdleGL_ ()
{
    glutRender::Instance.IdleGL ();
}

void glutRender::DisplayGL_ ()
{
    glutRender::Instance.DisplayGL ();
}

void glutRender::KeyboardGL_ (unsigned char c, int x, int y)
{
    glutRender::Instance.KeyboardGL (c, x, y);
}


void glutRender::ReshapeGL_ (int w, int h)
{
    glutRender::Instance.ReshapeGL (w, h);
}

void glutRender::setFocus(Widget *widget) {
    this->unsetFocus();
    widget->isFocused = true;
    this->focusedWidget = widget;
}

void glutRender::unsetFocus() {
    if (this->focusedWidget != NULL) {
        this->focusedWidget->isFocused = false;
        this->focusedWidget = NULL;
    }
}

void glutRender::MouseGL(int button, int state, int x, int y) {
    if (state == 0) {
        Widget *widget = NULL;
        for (std::vector<Widget *>::iterator it = this->widgets.begin(); it != this->widgets.end(); ++it) {
            if ((*it)->hasPoint(x, y)) {
                widget = *it;
            }
        }
        if (widget != NULL) {
            setFocus(widget);
            widget->isPressed = true;
            this->dragState = true;
            this->dragX = x - round(widget->x);
            this->dragY = y + round(widget->y);
        } else {
            this->unsetFocus();
        }
    } else {
        this->dragState = false;
        if (focusedWidget != NULL) focusedWidget->isPressed = false;
    }
}

void glutRender::MouseGL_(int button, int state, int x, int y) {
    glutRender::Instance.MouseGL(button, state, x, y);
}

void glutRender::MotionGL_(int x, int y) {
    glutRender::Instance.MotionGL(x, y);
}

void glutRender::MotionGL(int x, int y) {
    if (this->dragState && this->focusedWidget != NULL) {
        this->focusedWidget->x = x - this->dragX;
        this->focusedWidget->y = -(y - this->dragY);
    }
}
