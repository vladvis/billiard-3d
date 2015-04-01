#ifndef BILLIARD_TABLE
#define BILLIARD_TABLE

#define MINTIME 0.000001
#define EPS 0.00000001

#include "vec.h"
const double g = 9.8036;

class Table{
public:
	double bb;//ball-ball friction
	double e;//ball-ball restitution
	double je;//ball-table restitution TODO
	double d;//ball-table rolling friction
	double s;//ball-table spinning friction
	double f;//ball-table sliding friction
	double re;//ball-rail restitution TODO

	Table(const char * cfgfileName);
};

class Ball{
public:
	double a;//radius
	double m;//mass

	vec r;
	vec v;
	vec w;
public:
	Ball(const char * cfgfileName, vec r, vec v, vec w);
	int Collide(Table t, Ball &);
	int NextStep(Table t);//Return 0 is stops
	double Distance(Ball);
};

#endif
