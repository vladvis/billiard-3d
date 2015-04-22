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

const float EPS = 0.0000001;
const float G = 9.801;
const float g = 9.8036;

class Ball;

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

    float MINTIME;//mintime used in physics
    float CLOCK;//how much time will be NextStep will be executed during frame
    float FPS;
    float SLOWFACTOR;//IGT/RTA

    std::vector <Ball> balls;

	Table(const std::string cfgfileName);
	Table();
	int NextStep();
};

class Ball{
public:
	float a;//radius
	float m;//mass

	vec r;
	quat phi;
	vec v;
	vec w;

    GLuint texture;

	Ball(const std::string cfgfileName, vec r, quat phi, vec v, vec w, const char *texture_filename);
	int Collide(Table t, Ball &);
	int BoardCollide(Table t);
	float Distance(Ball);
	int NextStep(Table t, float mintime);//Return 0 is stops
};


#endif
