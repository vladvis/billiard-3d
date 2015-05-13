#ifndef BILLIARD_TABLE
#define BILLIARD_TABLE

#define _USE_MATH_DEFINES

#include "vec.h"
#include "quat.h"
#include "GlutHFunc.h"
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

const float EPS = 0.0000001;
const float G = 9.8036;

class Ball;

struct sndvolume{
	double collide=0, board=0, table=0;
};

class Table{
public:
	float bb;//ball-ball friction
	float e;//ball-ball restitution
	float je;//ball-table restitution
	float d;//ball-table rolling friction
	float s;//ball-table spinning friction
	float f;//ball-table sliding friction
	float rf;//ball-rail friction
	float re;//ball-rail restitution

    float lenx;//half size
    float leny;
    float border_height;

	unsigned texture = 0;
	unsigned tree_texture = 0;

    float MINTIME;//mintime used in physics
    float DEFCLOCK;//default num of time for each NextStep()
	float MULT;//mult for default clock (TimeLapse)
    float FPS;
    float SLOWFACTOR;//IGT/RTA

    int sc_b_num;

    std::vector <Ball> balls;

	Table(const std::string &cfgfileName);
	Table();
	int NextStep(sndvolume &snd);

	std::vector<std::vector<Ball *>> CollideDFS();
};

class Ball{
public:
	float a;//radius
	float m;//mass

	vec r;
	quat phi;
	vec v;
	vec w;
	std::vector <vec> track;

    char isvalid = 1;

    GLuint texture = 0;

	Ball(const std::string &cfgfileName, vec r, quat phi, vec v, vec w, const char *texture_filename);
	Ball();
	int CollideDFS(Table &t);
	int Collide(Table &t, Ball &, sndvolume &snd);
	int BoardCollide(Table &t, sndvolume &snd);
	float Distance(Ball &);
	int NextStep(Table &t, float mintime, sndvolume &snd);//Return 0 is stops
	void CollideDFS(Table &t, int *dfsed, int color);
	int noCollide(Ball &b);
};

#endif
