#ifndef BILLIARD_VEC
#define BILLIARD_VEC

#include <cmath>

class float3f
{
public:
	float x, y, z;
	float3f (float _x = 0, float _y = 0, float _z = 0) :
			x(_x), y(_y), z(_z) {};
};


class vec{
public:
	float x;
	float y;
	float z;

	vec();
	vec(float x, float y, float z);

	float mod();

	vec operator+ (vec b);
	vec operator+= (vec b);

	vec operator- (vec b);
	vec operator-= (vec b);

	vec normalized();

	friend vec operator* (float a, vec b);
	friend vec operator* (vec b, float a);

	friend vec operator/ (float a, vec b);
	friend vec operator/ (vec b, float a);

	float operator*(vec b); //Scalar product
	vec operator^(vec b); //Vector product

};

#endif
