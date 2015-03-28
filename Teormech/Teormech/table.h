#ifndef BILLIARD_TABLE
#define BILLIARD_TABLE

#define MINTIME 0.0001
#define EPS 0.000001

#include "vec.h"
const double g = 9.8036;

class Table{
public:
	double bb;//ball-ball friction
	double e;//ball-ball restituion
	double je;//ball-table restitution
	double d;//ball-table rolling friction
	double s;//ball-table spinning friction
	double f;//ball-table sliding friction
	double re;//ball-rail restituion

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
	void Collide(Ball);
	void NextStep(Table t);
	double Distance(Ball);
};

#endif