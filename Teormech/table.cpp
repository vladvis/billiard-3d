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

int Ball::Collide(Table t, Ball &b)
{
	if (Distance(b) > b.a + a) return 0;//No collide for you

    double hi = 5.0/2.0;

    vec k = (b.r - r).normalized();

    double v1n = v * k;
    double v2n = b.v * k;

    if (v2n >= v1n) return 0;//They don't move to each other :(

    vec v1t = v - v1n * k;
    vec v2t = b.v - v2n * k;

    //Normal components is not affected by friction - only restitution
    double v1n_n = (t.e*(v2n-v1n) + (v1n+v2n))/2;
    double v2n_n = (t.e*(v1n-v2n) + (v1n+v2n))/2;

    vec u = b.v - v + a * ((b.w - w) ^ k) - k * ((b.v - v) * k);
    double mu = u.mod();
    vec tau = u.normalized();

    double itr_v = t.bb * (1 + t.e) * (v1n - v2n) / 2;

    vec itr;//We use that friction vector is constant in it's direction - Coriolis
    if (2 * itr_v < mu){ //Always friction
        itr = itr_v * tau;
    }else{//Friction stops when u=0
        itr = u / 2;
    }

    w -= hi / a * (itr ^ k);
    b.w -= hi / a * (itr ^ k);

    v1t += itr;
    v2t -= itr;

    v = v1t + k * v1n_n;
    b.v = v2t + k * v2n_n;

    v.z = 0;
    b.v.z = 0;

    u = b.v - v + a * ((b.w - w) ^ k) - k * ((b.v - v) * k);
    return 1;
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

	double J = 2.0/5.0*a*a;

	if (std::abs(w.z) > EPS){
        double dwz = -t.s*g/J * (w.z>0 ? 1 : -1)*MINTIME;
		w.z += dwz;
		ret = 1;
	}

    if ((isnan(alpha)) && (isnan(beta))) return ret; //it means that v=0

	if (!isnan(alpha) && mu > EPS){ //Slippage -> sliding friction
        ret += 2;
        if (!isnan(beta) && mw > EPS){ //Rolling -> rolling friction
            ret += 4;
            double dbeta = (t.f*g*a/J/mw*cos(beta-alpha))*MINTIME;
            beta += dbeta;

            double dalpha = -t.d*g*a/J/mu*cos(beta-alpha)*MINTIME;
            alpha += dalpha;

            double dw = (t.f*g*a/J*sin(beta-alpha)-t.d*g/J)*MINTIME;
            mw += dw;
            if (mw < 0) mw=0; //Solvability check

            double du = (t.d*g*a/J*sin(beta-alpha) - t.f*g*(1+a*a/J))*MINTIME;
            mu += du;
            if (mu < 0) mu=0; //Solvability check

            u.x = mu * cos(alpha);
            u.y = mu * sin(alpha);
            w.x = mw * cos(beta);
            w.y = mw * sin(beta);
        }else{ //No rolling -> start of rolling TODO
            double du = (-t.f*g*(1+a*a/J))*MINTIME;
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
        double dw = -t.d*g/J*MINTIME;
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
	return sqrt((r.x - b.r.x)*(r.x - b.r.x) + (r.y - b.r.y)*(r.y - b.r.y) + (r.z - b.r.z)*(r.z - b.r.z));
}

