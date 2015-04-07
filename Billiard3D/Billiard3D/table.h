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


const double EPS = 0.00000001;
const double G = 9.801;
const double g = 9.8036;

class Ball;

class Table{
public:
	double bb;//ball-ball friction
	double e;//ball-ball restitution
	double je;//ball-table restitution TODO
	double d;//ball-table rolling friction
	double s;//ball-table spinning friction
	double f;//ball-table sliding friction
	double rf;//ball-rail friction
	double re;//ball-rail restitution

    double lenx;//half size
    double leny;

    double MINTIME;//mintime used in physics
    double CLOCK;//how much time will be NextStep will be executed during frame
    double FPS;
    double SLOWFACTOR;//IGT/RTA

    unsigned texture;

    std::vector <Ball> balls;

	Table(const std::string cfgfileName);
	Table();
	int NextStep();
};

class Ball{
public:
	double a;//radius
	double m;//mass

	vec r;
	quat phi;
	vec v;
	vec w;

    GLuint texture;

	Ball(const std::string cfgfileName, vec r, quat phi, vec v, vec w);
	int Collide(Table t, Ball &);
	int BoardCollide(Table t);
	double Distance(Ball);
	int NextStep(Table t, double mintime);//Return 0 is stops
};


#endif
