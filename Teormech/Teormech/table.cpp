#include <cmath>
#include <fstream>
#include "table.h"

Table::Table(const char * name){
	std::ifstream file(name);

	double x,y;

	if (file.is_open())
	{
		file >> bb >> e >> je >> d >> s >> f >> re;
	}

	file.close();
};

Ball::Ball(const char * name, vec r, vec v, vec w) :
	r(r),
	v(v),
	w(w)
{
	std::ifstream file(name);

	if (file.is_open())
	{
		file >> a >> m;
	}

	file.close();
};

void Ball::Collide(Ball b)
{
	if (Distance(b) > b.a + a) return;
};

void Ball::NextStep(Table t)
{
	vec k = vec(0,0,1);
	vec u = v + a * k ^ w;

	double mu = sqrt(u.x*u.x + u.y*u.y);
	double mw = sqrt(w.x*w.x + w.y*w.y);
	double wz = w.z;

	double alpha = acos(u.x/mu) + (M_PI ? u.x<0 : 0);
	double beta = acos(w.x/mw) + (M_PI ? u.x<0 : 0);

	double J = 2/5*m*a*a;

	if (std::abs(wz) < EPS){
		double dwz = -t.s*m*g * (1 ? w.z>0 : -1);
		wz += dwz;
	}

	if (mu < EPS){
		double du = (-t.f*g*(1+m*a*a/J) + t.d*m*g*a/J*sin(beta-alpha))*MINTIME;
		mu += du;
	}

	double dalpha = -t.d*m*g*a/J/mu*cos(beta-alpha)*MINTIME;
	alpha += dalpha;

	double dw = t.f*m*g*a/J*sin(beta-alpha) - t.d*m*g/J;
	mw += dw;

	double dbeta = t.f*m*g*a/J/mw*cos(beta-alpha);
	beta += dbeta;


	r += (MINTIME * v);
};

double Ball::Distance(Ball b)
{

	return sqrt(r * b.r);
}

