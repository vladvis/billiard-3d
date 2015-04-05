#include "quat.h"

quat::quat() {};

quat::quat(double l, vec v): l(l), v(v) {};

quat operator* (double a, quat b)
{
	return quat(a*b.l, a*b.v);
};

quat operator* (quat b, double a)
{
	return quat(a*b.l, a*b.v);
};

quat operator/ (double a, quat b)
{
	return quat(a/b.l, a/b.v);
};

quat operator/ (quat b, double a)
{
	return quat(b.l/a, b.v/a);
};

quat quat::operator*(quat b)
{
	return quat(l*b.l - v*b.v, l*b.v + b.l*v + (v^b.v));
};

quat quat::operator+(quat b)
{
	return quat(l+b.l, v+b.v);
};

quat quat::operator+=(quat b)
{
	l += b.l; v += b.v;
	return *this;
};

quat quat::operator-(quat b)
{
	return quat(l-b.l, v-b.v);
};

quat quat::operator-=(quat b)
{
	l -= b.l; v -= b.v;
	return *this;
};

double quat::mod()
{
	return sqrt(l*l + v.x*v.x + v.y*v.y + v.z*v.z);
};

quat quat::normalized()
{
    return (*this) / this->mod();
};
