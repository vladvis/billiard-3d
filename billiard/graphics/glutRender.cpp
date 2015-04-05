#include "glutRender.h"

GLuint ball_texture;

/* there is only one copy of glutRender, so we are have to use this lifehack to deal with C GLUT library on C++, it is ok */
glutRender glutRender::Instance;

void glutRender::Init (int* argc, char* argv[], const char *table_config, const char *balls_config)
{
    srand(time(0));

    multipluer = 2.5f;
    alpha = 1.8*3.14/3;
    curre_ball = 0;
    cam_height_h = 1.5;


    if (*argc > 1)
        start_state_config_filename = std::string(argv[1]);
    else
        start_state_config_filename = std::string("lunev");

    glutInit (argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    int ScreenWidth = glutGet (GLUT_SCREEN_WIDTH);
    int ScreenHeight = glutGet (GLUT_SCREEN_HEIGHT);

    glutInitWindowPosition ((ScreenWidth - WindowWidth) / 2, (ScreenHeight - WindowHeight) / 2);

    glutInitWindowSize (WindowWidth, WindowHeight);

    glutWindowHandle = glutCreateWindow ("Billiard 3D Project - [q]uit");
    assert (glutWindowHandle != 0);

#ifdef FULLSCREEN
    glutGameModeString ("1920x1080:32@60");
    glutEnterGameMode();
#endif

    glutDisplayFunc (DisplayGL_);
    glutIdleFunc (IdleGL_);
    glutMouseFunc (MouseGL_);
    glutMotionFunc (MotionGL_);
    glutKeyboardFunc (KeyboardGL_);
    glutReshapeFunc (ReshapeGL_);


    // ----- OpenGL settings -----
    glEnable (GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set out clear colour to black, full alpha
    glClearDepth(1.0f);  // Clear the entire depth of the depth buffer

    glShadeModel (GL_SMOOTH); // Enable (gouraud) shading

    glDepthFunc(GL_LEQUAL); // Set our depth function to overwrite if new value less than or equal to current value
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Ask for nicest perspective correction
    glEnable(GL_CULL_FACE); // Do not draw polygons facing away from us

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); //TODO: GL_LIGHT_MODEL_LOCAL_VIEWER
    // ----------------------------

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_AUTO_NORMAL);
    /* fog */
    glEnable(GL_FOG);
    GLfloat fogColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 15);
    glFogf(GL_FOG_END, 40);
    glFogfv(GL_FOG_COLOR, fogColor);
    /* fog end */

    checkGLError ("glutRender::Init");

    table_config_filename = std::string(table_config);
    balls_config_filename = std::string(balls_config);
    LoadConfig (table_config_filename, balls_config_filename, start_state_config_filename);
    PreviousTicks = std::clock();

    ball_texture = LoadTexture("textures/9.bmp");

    glutMainLoop ();

    Cleanup(); //guess, is never calling
}


void glutRender::LoadConfig(const std::string table_config, const std::string balls_config, const std::string start_state_config)
{
    calculations_started = false;
    GameTable = Table(table_config);

    std::ifstream file(start_state_config.c_str());
    double px, py, pz, rx, ry, rz, vx, vy, vz, wx, wy, wz;
    if (!file.is_open())
    {
        std::cout << "Failed to open file!";
        return;
    }
    while(file >> rx >> ry >> rz >> px >> py >> pz >> vx >> vy >> vz >> wx >> wy >> wz)
        glutRender::GameTable.balls.push_back(Ball(balls_config, vec(rx, ry, rz), vec(px, py, pz), vec(vx, vy, vz), vec(wx, wy, wz)));

    if (GameTable.balls.empty())
    {
        std::cout << "Больше мячей богу мячей!";
        return;
    }

    std::cout << "Loaded configurations " << table_config << "; " << balls_config << std::endl;
}

void glutRender::DisplayGL ()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // sensative
    glLoadIdentity();
    gluLookAt (GameTable.balls[curre_ball].r.x + multipluer*sin(alpha), GameTable.balls[curre_ball].a + cam_height_h*log(multipluer),GameTable.balls[curre_ball].r.y + multipluer*cos(alpha),
               GameTable.balls[curre_ball].r.x, GameTable.balls[curre_ball].r.z, GameTable.balls[curre_ball].r.y,
               0.0f, 1.0f, 0.0f);

    glDisable(GL_LIGHTING);
    DrawGroundGrid (-3);
    glEnable(GL_LIGHTING);

    init_l();
    DrawBilliardTable (GameTable.lenx, GameTable.leny,  0.1f, 6, GameTable.texture, 0);

    const GLfloat ball_r = GameTable.balls[0].a;

/*
glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (g_texture > 0)
        {
            glEnable (GL_TEXTURE_2D);
            glBindTexture (GL_TEXTURE_2D, g_texture);
        } else glColor3ub(0, 150, 0);

        DrawNiceRectangle(-hwidth, hwidth, -hheight, hheight);
        glDisable(GL_TEXTURE_2D);

    */
    for(std::vector<Ball>::iterator it = GameTable.balls.begin(); it != GameTable.balls.end(); ++it)
    {
        glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, ball_texture);


                if (curre_ball != it - GameTable.balls.begin())
                    glColor3f (1.0f, 1.0f, 1.0f);
                else
                    glColor3f (1.0f, 0.0f, 0.0f);

                glTranslatef(it->r.x, ball_r + it->r.z, it->r.y);
                glRotatef(it->phi.x, 1, 0, 0);
                glRotatef(it->phi.y, 0, 1, 0);
                glRotatef(it->phi.z, 0, 0, 1);
                glutSolidSphere (ball_r, 40, 40);
                glDisable(GL_TEXTURE_2D);
            glPopMatrix ();
        glPopAttrib();
    }

    glFlush();

    glutSwapBuffers ();
}

void glutRender::IdleGL ()
{
    std::clock_t CurrentTicks = std::clock ();
    //float deltaTicks = (CurrentTicks - PreviousTicks);
    PreviousTicks = CurrentTicks;

    if (calculations_started)
    {
        const double MINTIME = 0.000001;
        for (int i = 0; i < 15000; i++)
            GameTable.NextStep(MINTIME);
    }
    //float fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC; // TODO: fps here

    glutPostRedisplay ();
}

void glutRender::MouseGL (int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
    {
        if (state == GLUT_UP)
        {
//				MouseManipulator.LeftKeyPressed = false;
        }
        else
        {
            //			MouseManipulator.LeftKeyPressed = true;
        }
    }
    break;

    case GLUT_RIGHT_BUTTON:
    {
        //TODO: make hit the cue here
    }
    break;
    }

    glutPostRedisplay ();
}

void glutRender::MotionGL (int x, int y)
{
    // TODO: normal mouse reaction
//	if (MouseManipulator.LeftKeyPressed)
    {

    }
}

void glutRender::KeyboardGL (unsigned char c, int x, int y)
{
    switch (c)
    {
    //TODO: the following is the test
    case 'g':
    case 'G':
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


    case '\033': // escape quits
    case 'Q':
    case 'q':
    {
        // Cleanup up and quit
        Cleanup();
    }
    break;

    case  'D':
    case  'd':
    {
        alpha += 0.05f;
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
        alpha -= 0.05f;
        if (alpha <= 0) alpha += 2*M_PI;
    }
    break;


    case 'S':
    case 's':
    {
        if (multipluer < 6.8) multipluer += 0.2f;
    }
    break;

    case 'W':
    case 'w':
    {
        if (multipluer > 1.4) multipluer -= 0.2f;
    }
    break;

    case '\t': //TAB
    {
        curre_ball = (curre_ball + 1) % GameTable.balls.size();
    }
    break;

    case 'R':
    case 'r':
    {
        LoadConfig (table_config_filename, balls_config_filename, start_state_config_filename);
    }
    break;

    case ' ':
    {
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
        if (cam_height_h > 0.6) cam_height_h -= 0.2f;
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
    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay ();
}

void glutRender::Cleanup ()
{
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

void glutRender::MouseGL_ (int button, int state, int x, int y)
{
    glutRender::Instance.MouseGL (button, state, x, y);
}

void glutRender::MotionGL_ (int x, int y)
{
    glutRender::Instance.MotionGL (x, y);
}

void glutRender::ReshapeGL_ (int w, int h)
{
    glutRender::Instance.ReshapeGL (w, h);
}
