#ifndef BILLIARD_QUAT
#define BILLIARD_QUAT

#include "vec.h"
#include <cmath>

class quat{
public:
	float l;
	vec v;

	quat();
	quat(float l, vec v);

    float mod();
    quat normalized();

	quat operator+ (quat b);
	quat operator+= (quat b);

	quat operator- (quat b);
	quat operator-= (quat b);

	friend quat operator* (float a, quat b);
	friend quat operator* (quat b, float a);

	friend quat operator/ (float a, quat b);
	friend quat operator/ (quat b, float a);

	quat operator*(quat b);
};

#endif
