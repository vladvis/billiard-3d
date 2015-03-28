#include <cmath>
#include "vec.h"

vec::vec() {};

vec::vec(double x, double y, double z) :
	x(x), y(y), z(z) {};

vec operator* (double a, vec b)
{
	return vec(a*b.x, a*b.y, a*b.z);
};

double vec::operator*(vec b)
{
	return x*b.x + y*b.y + z*b.z;
};

vec vec::operator^(vec b)
{
	return vec(y*b.z - y*b.z, - x*b.z + x*b.z, x*b.y - x*b.y);
};

vec vec::operator+(vec b)
{
	return vec(x+b.x, y+b.y, z+b.z);
};

vec vec::operator+=(vec b)
{
	x += b.x; y += b.y; z += b.z;
};

double vec::mod()
{
	return sqrt(x*x + y*y + z*z);
};