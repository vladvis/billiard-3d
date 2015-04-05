#ifndef BILLIARD_BALL
#define BILLIARD_BALL

#define EPS 0.00000001
#define G 9.801

#include "../math/vec.h"
#include "table.h"
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>

class Table;

class Ball{
public:
	double a;//radius
	double m;//mass

	vec r;
	vec phi;
	vec v;
	vec w;

	Ball(const std::string cfgfileName, vec r, vec phi, vec v, vec w);
	int Collide(Table t, Ball &, double mintime);
	int BoardCollide(Table t);
	double Distance(Ball);
	int NextStep(Table t, double mintime);//Return 0 is stops
};

#endif // BILLIARD_BALL

