#include <cmath>
#include <fstream>
#include <iostream>
#include "table.h"

#define isnan(f) (f != f)
#define sign(f) (f>0 ? 1 : -1);

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

int Ball::NextStep(Table t)
{
    int ret = 0;
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
		ret = 1;
	}

    //std::cout << alpha << " " << beta << std::endl;
    if ((isnan(alpha)) && (isnan(beta))) return ret; //it means that v=0

	if (!isnan(alpha) && mu > EPS){ //Slippage -> sliding friction
        ret += 2;
        if (!isnan(beta) && mw > EPS){ //Rolling -> rolling friction
            ret += 4;
            double dbeta = (t.f*m*g*a/J/mw*cos(beta-alpha))*MINTIME;
            beta += dbeta;

            double dalpha = -t.d*m*g*a/J/mu*cos(beta-alpha)*MINTIME;
            alpha += dalpha;

            double dw = (t.f*m*g*a/J*sin(beta-alpha)-t.d*m*g/J)*MINTIME;
            mw += dw;
            if (mw < 0) mw=0; //Solvability check

            double du = (t.d*m*g*a/J*sin(beta-alpha) - t.f*g*(1+m*a*a/J))*MINTIME;
            mu += du;
            if (mu < 0) mu=0; //Solvability check

            u.x = mu * cos(alpha);
            u.y = mu * sin(alpha);
            w.x = mw * cos(beta);
            w.y = mw * sin(beta);
        }else{ //No rolling -> start of rolling TODO
            double du = (-t.f*g*(1+m*a*a/J))*MINTIME;
            mu += du;
            if (mu < 0) mu=0; //Solvability check

            double dw = (t.f*g*a)*MINTIME;
            mw += dw;
            if (mw < 0) mw=0; //Solvability check

            u.x = mu * cos(alpha);
            u.y = mu * sin(alpha);
            w.x = mw * sin(alpha);
            w.y = mw * (-cos(alpha));
        }
	}else{ //No slippage -> no sliding friction, only rolling
	    ret += 4;
        double dw = -t.d*m*g/J*MINTIME;
        mw += dw;
        if (mw < 0) mw=0; //Solvability check

        w.x = mw * cos(beta);
        w.y = mw * sin(beta);
	}

	v = u - a * (k ^ w);
	r += (MINTIME * v);

	return ret;
};

double Ball::Distance(Ball b)
{

	return sqrt(r * b.r);
}

