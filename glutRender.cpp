#include "glutRender.h"

GLUquadricObj *sphere = NULL;

const int hel_info_lines_number = 13;
char help_info[hel_info_lines_number][70] =
                        {"[ q/Q/Esc ] - exit",
                        "[ Tab ] - switch between objects",
                        "[ c/C ] - set view on table center",
                        "[ Spacebar ] - begin/pause computational experiment",
                        "[ r/R ] - reset scene",
                        "[ w/W ] / [ s/S ] - zoom in/out camera from the active object",
                        "[ a/A ] / [ d/D ] - rotate the camera left/right on the active object",
                        "[ - ] / [ + ] - raise/lower the camera relative to the table",
                        "[ p/P ] / [ l/L ] - disable/enable the surfaces drawing",
                         "[ ] ] / [ ] ] - decrease/increase time rate",
                        "[ m/M ] - turn off music",
                        "",
                        "Designed by Kopyrin Denis, Shcherbatov Kirill, Vladas Bulavas"
                        };

/* there is only one copy of glutRender, so we are have to use this lifehack to deal with C GLUT library on C++, it is ok */
glutRender glutRender::Instance;

void glutRender::addBall() {
    int fd[2];

    if (pipe (fd))
    {
        fprintf(stderr,"Pipe failed.\n");
        exit(EXIT_FAILURE);
    }
    char cmd[256];
    sprintf(cmd, "python2 dialog.py %d", fd[1]);
    int pid = fork();
    if (pid == 0) {
        if (system(cmd)) std::cout << "Can't create window!" << std::endl;
        exit(0);
    }
    FILE *stream;
    stream = fdopen(fd[0], "r");
    int status;
    if(fscanf(stream, "%d", &status)) return;
    if (status == 1) {
        float x,y,z,vx,vy,vz,vax,vay,vaz;
        if(fscanf(stream, "%f%f%f%f%f%f%f%f%f", &x, &y, &z, &vx, &vy, &vz, &vax, &vay, &vaz)) return;
        float phi = (rand() % 100) / 50.0 * M_PI;
        GameTable.balls.push_back(Ball("ball.cfg", vec(x, y, z), quat(cos(phi), sin(phi) * vec(rand(), rand(), rand()).normalized()), vec(vx, vy, vz), vec(vax, vay, vaz), "textures/02.data"));

    } else {
        printf("Cancel");
    }
    fclose (stream);
}

void glutRender::Init (int* argc, char* argv[], const char *table_config, const char *balls_config, const char *sounds_config)
{
    srand(time(0));

    multipluer = 2.5f;
    alpha = 1.8*3.14/3;
    curre_ball = 0;
    cam_height_h = 1.4;

    help_menu_showed = false;
    calculations_started = false;

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
    glutReshapeFunc (ReshapeGL_);

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

    MainTheme.Play(0.4f);
    main_theme_state = true;

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
    while(file >> rx >> ry >> rz >> vx >> vy >> vz >> wx >> wy >> wz >> texture_filename)
    {
        float phi = (rand() % 100) / 50.0 * M_PI;
        glutRender::GameTable.balls.push_back(Ball(balls_config, vec(rx, ry, rz), quat(cos(phi), sin(phi) * vec(rand(), rand(), rand()).normalized()), vec(vx, vy, vz), vec(wx, wy, wz), texture_filename));
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

    /* text out */
    {
        setOrthographicProjection();
        glPushMatrix();
            glLoadIdentity();
            glScalef(1.0f, -1.0f, 1.0f);
            glColor3f(0.8f, 0.8f, 0.7f);

            char info[250];

            if (curre_ball < (int)GameTable.balls.size())
            {
                sprintf(info, "BALL[%d] state: r(%+0.2f, %+0.2f, %+0.2f); v(%+0.2f, %+0.2f, %+0.2f); w(%+0.2f, %+0.2f, %+0.2f)", curre_ball+1,
                        GameTable.balls[curre_ball].r.x, GameTable.balls[curre_ball].r.y, GameTable.balls[curre_ball].r.z,
                        GameTable.balls[curre_ball].v.x,GameTable.balls[curre_ball].v.y, GameTable.balls[curre_ball].v.z,
                        GameTable.balls[curre_ball].w.x, GameTable.balls[curre_ball].w.y, GameTable.balls[curre_ball].w.z);

                glPushMatrix();
                    //glColor3f(1.0f, 1.0f, 1.0f);
                    glTranslatef(15.0f, -WindowHeight + 15, 0.0f);
                    renderString(GLUT_STROKE_ROMAN, info);
                glPopMatrix();
            }

            glPushMatrix();
                //glColor3f(0.0f, 1.0f, 0.0f);
                glTranslatef(WindowWidth - 260.0f, -65.0f, 0.0f);
                sprintf(info, "Scores    %d", GameTable.sc_b_num);
                renderString(GLUT_STROKE_ROMAN, info);
            glPopMatrix();

            glPushMatrix();
               //glColor3f(1.0f, 1.0f, 0.0f);
               glTranslatef(WindowWidth - 260.0f, -30.0f, 0.0f);
               sprintf(info, "Time rate  %.3f", GameTable.MULT * 100);
               renderString(GLUT_STROKE_ROMAN, info);
            glPopMatrix();

            glPushMatrix();
                //glColor3f(0.8f, 0.8f, 0.7f);
                glTranslatef(15.0f, -30.0f, 0.0f);
                renderString(GLUT_STROKE_ROMAN, (char *)"Press \'h\' to show/hide help information");
            glPopMatrix();


            if (help_menu_showed)
            {
                //glColor3f(0.8f, 0.8f, 0.7f);

                glPushMatrix();
                    glTranslatef(35.0f, -65.0f, 0.0f);
                    float height = 0;
                    for (int i = 0; i<hel_info_lines_number; i++)
                    {
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
        gluLookAt (multipluer*sin(alpha), cam_height_h*log(multipluer), multipluer*cos(alpha),
                   0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f);

    const GLfloat ball_r = GameTable.balls[0].a;

    DrawGroundGrid (groundLevel);

    if (curre_ball < (int)GameTable.balls.size())
    {
        Ball &ActiveBall = GameTable.balls[curre_ball];

        glPushMatrix();
            glTranslatef(ActiveBall.r.y, 0.001f, ActiveBall.r.x);
            DrawCoolRoundAround(ball_r, 2*M_PI);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(ActiveBall.r.y, ball_r + ActiveBall.r.z, ActiveBall.r.x);
            glRotatef(360*acos(ActiveBall.phi.l)/M_PI, ActiveBall.phi.v.y, ActiveBall.phi.v.z, ActiveBall.phi.v.x);
            DrawVerticalPLine();
        glPopMatrix();
    }

    init_l();

	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		DrawBilliardTable (GameTable.lenx, GameTable.leny,  GameTable.border_height, 1.2*abs(groundLevel), ball_r);
	glPopMatrix();

    glColor3f (0.75f, 0.75f, 0.75f);
    for(std::vector<Ball>::iterator it = GameTable.balls.begin(); it != GameTable.balls.end(); ++it)
    {
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
            glBindTexture(GL_TEXTURE_2D, it->texture);

            glTranslatef(it->r.y, ball_r + it->r.z, it->r.x);
            glRotatef(360*acos(it->phi.l)/M_PI, it->phi.v.y, it->phi.v.z, it->phi.v.x);
            gluSphere(sphere, ball_r, 20, 20);

            glDisable(GL_TEXTURE_2D);
        glPopMatrix ();
    }

    glDisable(GL_LIGHTING);
	glPushMatrix();
        glColor3f(0.7f, 0.7f, 0.7f);
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
		for (int i = 0; i < GameTable.DEFCLOCK * GameTable.MULT; i++)
		{
			if (!(ret |= GameTable.NextStep())) calculations_started = false;
		}

        if (ret & 256)
            SoundController.Play(MediaLibrary["collide"]);
        if (ret & 512)
            SoundController.Play(MediaLibrary["pocket"]);
        if (ret & 1024)
            SoundController.Play(MediaLibrary["border_hit"]);
        if (ret & 16)
            SoundController.Play(MediaLibrary["border_hit"]);
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
        {
            alpha -= 0.2f;
            if (alpha <= 0) alpha += 2*M_PI;
        }
        break;

        case 'S':
        case 's':
        {
            if (multipluer < 3.0) multipluer += 0.2f;
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
            else
                SoundController.Play(MediaLibrary["wrong"]);
        }
        break;

        case 'R':
        case 'r':
        {
            SoundController.Play(MediaLibrary["wrong"]);
            LoadConfig (table_config_filename, balls_config_filename, start_state_config_filename);
        }
        break;

        case 'z':
        case 'Z':
        {
            if (!calculations_started)
            {
                SoundController.Play(MediaLibrary["menu"]);
                addBall();
            }
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

        case '}':
        case ']':
        {
            if (GameTable.MULT < 4)
                if (GameTable.MULT < 1)
                    GameTable.MULT *= 2;
                else
                    GameTable.MULT += 0.05f;
        }
        break;

        case '{':
        case '[':
        {
            if (GameTable.MULT <= 1)
                GameTable.MULT /= 2;
            else
                GameTable.MULT -= 0.05;
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
