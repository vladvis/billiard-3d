#ifndef BILLIARD_VEC
#define BILLIARD_VEC

#include <cmath>

class vec{
public:
	double x;
	double y;
	double z;

	vec();
	vec(double x, double y, double z);

	double mod();

	vec operator+ (vec b);
	vec operator+= (vec b);

	vec operator- (vec b);
	vec operator-= (vec b);

	vec normalized();

	friend vec operator* (double a, vec b);
	friend vec operator* (vec b, double a);

	friend vec operator/ (double a, vec b);
	friend vec operator/ (vec b, double a);

	double operator*(vec b); //Scalar product
	vec operator^(vec b); //Vector product

};

#endif
