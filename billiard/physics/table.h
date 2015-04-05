#ifndef BILLIARD_TABLE
#define BILLIARD_TABLE

#define EPS 0.00000001
#define G 9.801

#include "../math/vec.h"
#include "ball.h"
#include <fstream>
#include <vector>

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

    std::vector <Ball> balls;

	Table(const std::string cfgfileName);
	Table();
	int NextStep(double mintime);
};

#endif
