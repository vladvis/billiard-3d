#ifndef BILLIARD_QUAT
#define BILLIARD_QUAT

#include "vec.h"
#include <cmath>

class quat{
public:
	double l;
	vec v;

	quat();
	quat(double l, vec v);

    double mod();
    quat normalized();

	quat operator+ (quat b);
	quat operator+= (quat b);

	quat operator- (quat b);
	quat operator-= (quat b);

	friend quat operator* (double a, quat b);
	friend quat operator* (quat b, double a);

	friend quat operator/ (double a, quat b);
	friend quat operator/ (quat b, double a);

	quat operator*(quat b);
};

#endif
