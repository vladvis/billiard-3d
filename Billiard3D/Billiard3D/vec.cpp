#include "vec.h"

vec::vec() {};

vec::vec(double x, double y, double z) :
	x(x), y(y), z(z) {};

vec operator* (double a, vec b)
{
	return vec(a*b.x, a*b.y, a*b.z);
};

vec operator* (vec b, double a)
{
	return vec(a*b.x, a*b.y, a*b.z);
};

vec operator/ (double a, vec b)
{
	return vec(a/b.x, a/b.y, a/b.z);
};

vec operator/ (vec b, double a)
{
	return vec(b.x/a, b.y/a, b.z/a);
};

double vec::operator*(vec b)
{
	return x*b.x + y*b.y + z*b.z;
};

vec vec::operator^(vec b)
{
	return vec(y*b.z - b.y*z, b.x*z -x*b.z, x*b.y - b.x*y);
};

vec vec::operator+(vec b)
{
	return vec(x+b.x, y+b.y, z+b.z);
};

vec vec::operator+=(vec b)
{
	x += b.x; y += b.y; z += b.z;
	return *this;
};

vec vec::operator-(vec b)
{
	return vec(x-b.x, y-b.y, z-b.z);
};

vec vec::operator-=(vec b)
{
	x -= b.x; y -= b.y; z -= b.z;
	return *this;
};

double vec::mod()
{
	return sqrt(x*x + y*y + z*z);
};

vec vec::normalized()
{
    return (*this) / this->mod();
};
