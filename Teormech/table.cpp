#include <cmath>
#include <fstream>
#include <iostream>
#include "table.h"

Table::Table(const char * name){
	std::ifstream file(name);

	if (file.is_open())
	{
		file >> bb >> e >> je >> d >> s >> f >> re;
	}
	else
    {
        throw;
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
	else
    {
        throw;
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
	vec u = v + a * (k ^ w);

	double mu = sqrt(u.x*u.x + u.y*u.y);
	double mw = sqrt(w.x*w.x + w.y*w.y);

	double alpha = acos((u.y<0 ? -1 : 1) * u.x/mu) + (u.y<0 ? M_PI : 0);
	double beta =  acos((w.y<0 ? -1 : 1) * w.x/mw) + (w.y<0 ? M_PI : 0);

	double J = 2.0/5.0*m*a*a;

	if (std::abs(w.z) > EPS){
		double dwz = -t.s*m*g * (w.z>0 ? 1 : -1)*MINTIME;
		w.z += dwz;
	}

	if (mu > EPS){
		double du = (-t.f*g*(1+m*a*a/J) + t.d*m*g*a/J*sin(beta-alpha))*MINTIME;
		mu += du;

		double dalpha = -t.d*m*g*a/J/mu*cos(beta-alpha)*MINTIME;
		alpha += dalpha;

		u.x = mu * cos(alpha);
		u.y = mu * sin(alpha);

		double dw = (t.f*m*g*a/J*sin(beta-alpha) - t.d*m*g/J)*MINTIME;
		mw += dw;

		double dbeta = (t.f*m*g*a/J/mw*cos(beta-alpha))*MINTIME;
		beta += dbeta;

		w.x = mw * cos(beta);
		w.y = mw * sin(beta);

		v = u - a * (k ^ w);
	}

	r += (MINTIME * v);
};

double Ball::Distance(Ball b)
{

	return sqrt(r * b.r);
}

