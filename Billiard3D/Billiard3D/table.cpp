#include "table.h"

#define isnan(f) (f != f)
#define sign(f) (f>0 ? 1 : -1)

int Table::NextStep(){
	int ret = 0;

	//for (std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it)
    for (Ball& b: balls)
		b.BoardCollide(*this);

	for (auto it = balls.begin(); it != balls.end(); ++it)
		for (auto jt = it + 1; jt != balls.end(); ++jt)
			it->Collide(*this, *jt);

	for (Ball& b: balls){
		int retb = b.NextStep(*this, MINTIME);
		if (retb != 1) ret |= (retb);
	}

	return ret;
}

Table::Table(const std::string name): MINTIME(0), CLOCK(0), FPS(60), SLOWFACTOR(1){
	std::ifstream file(name.c_str());

	if (file.is_open())
	{
		file >> bb >> e >> je >> d >> s >> f >> rf >> re >> lenx >> leny;
	}
	else
	{
		throw;
	}

	file.close();
};

Table::Table(): MINTIME(0), CLOCK(0), FPS(60), SLOWFACTOR(1){};

Ball::Ball(const  std::string name, vec r, quat phi, vec v, vec w, const char *texture_filename) :
r(r),
phi(phi),
v(v),
w(w)
{
	std::ifstream file(name.c_str());

	if (file.is_open())
	{
	    file >> a >> m;
        texture = raw_texture_load(texture_filename, 500, 250);
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

	vec k = (b.r - r).normalized();

	double v1n = v * k;
	double v2n = b.v * k;

	if (v2n >= v1n) return 0;//They don't move to each other :(

	double hi = 5.0 / 2.0;
	vec v1t = v - v1n * k;
	vec v2t = b.v - v2n * k;

	//Normal components is not affected by friction - only restitution
	double v1n_n = (t.e*(v2n - v1n) + (v1n + v2n)) / 2;
	double v2n_n = (t.e*(v1n - v2n) + (v1n + v2n)) / 2;

	vec u = b.v - v + a * ((b.w + w) ^ k) - k * ((b.v - v) * k);
	double mu = u.mod();
	vec tau = u.normalized();

	double itr_v = t.bb * (1 + t.e) * (v1n - v2n) / 2;

	vec itr;//We use that friction vector is constant in it's direction - Coriolis
	if (2 * itr_v < mu){ //Always friction
		itr = itr_v * tau;
	}
	else{//Friction stops when u=0
		itr = u / 2;
	}

	w -= hi / a * (itr ^ k);
	b.w -= hi / a * (itr ^ k);

	v1t -= itr;
	v2t += itr;

	v = v1t + k * v1n_n;
	b.v = v2t + k * v2n_n;

	//v.z = 0;
	//b.v.z = 0;

	return 1;
};

int Ball::BoardCollide(Table t){ //TODO Collision of Rezal
	int ret = 0;

	vec k(0, 0, 0);

	if (std::abs(r.x) > t.lenx - a && sign(v.x) == sign(r.x))
	{
		k = vec(sign(r.x), 0, 0);
		ret = 1;
	}

	if (std::abs(r.y) > t.leny - a && sign(v.y) == sign(r.y))
	{
		k = vec(0, sign(r.y), 0);
		ret = 1;
	}

	if (ret){
		double hi = 2.0 / 5.0;
		double vn = v * k;
		vec vt = v - k * (v * k);

		vec u = v - k * (v * k) + a * (w ^ k);
		double itr_v = t.rf * (1 + t.re) * vn;

		vec itr;

		if (u.mod() < itr_v){
			itr = u;
		}
		else{
			itr = u.normalized() * itr_v;
		}

		vn = -t.re * vn;
		vt -= itr;

		v = vt + vn * k;
		w -= 1 / hi * (itr ^ k);
		ret = 1;
	}
	return ret;
}

int Ball::NextStep(Table t, double mintime)
{
	int ret = 0;
	if (std::abs(r.z) < EPS)
	{ //We are not very high : on cloth - moving
		vec k = vec(0, 0, 1);
		vec u = v + a * (k ^ w);

		double mu = sqrt(u.x*u.x + u.y*u.y);
		double mw = sqrt(w.x*w.x + w.y*w.y);

		double alpha = acos((u.y < 0 ? -1 : 1) * u.x / mu) + (u.y < 0 ? M_PI : 0);
		double beta = acos((w.y < 0 ? -1 : 1) * w.x / mw) + (w.y < 0 ? M_PI : 0);

		double J = 2.0 / 5.0*a*a;

		if (std::abs(w.z) > EPS){
			double dwz = -t.s*g / J * (w.z > 0 ? 1 : -1)*mintime;
			w.z += dwz;
			ret = 1;
		}

		if (((isnan(alpha)) && (isnan(beta)))){
			r += (mintime * v);
			phi += (quat(0, w) * phi) / 2 * mintime;
			phi = phi.normalized();
			return ret;
		}
		//it means that v=0 TODO L1

		if (!isnan(alpha) && mu > EPS){ //Slippage -> sliding friction
			ret += 2;
			if (!isnan(beta) && mw > EPS){ //Rolling -> rolling friction
				ret += 4;
				double dbeta = (t.f*g*a / J / mw*cos(beta - alpha))*mintime;
				beta += dbeta;

				double dalpha = -t.d*g*a / J / mu*cos(beta - alpha)*mintime;
				alpha += dalpha;

				double dw = (t.f*g*a / J*sin(beta - alpha) - t.d*g / J)*mintime;
				mw += dw;
				if (mw < 0) mw = 0; //Solvability check

				double du = (t.d*g*a / J*sin(beta - alpha) - t.f*g*(1 + a*a / J))*mintime;
				mu += du;
				if (mu < 0) mu = 0; //Solvability check

				u.x = mu * cos(alpha);
				u.y = mu * sin(alpha);
				w.x = mw * cos(beta);
				w.y = mw * sin(beta);
			}
			else{ //No rolling -> start of rolling TODO
				double du = (-t.f*g*(1 + a*a / J))*mintime;
				mu += du;
				if (mu < 0) mu = 0; //Solvability check

				double dw = (t.f*g*a)*mintime;
				mw += dw;
				if (mw < 0) mw = 0; //Solvability check

				u.x = mu * cos(alpha);
				u.y = mu * sin(alpha);
				w.x = mw * sin(alpha);
				w.y = mw * (-cos(alpha));
			}
		}
		else{ //No slippage -> no sliding friction, only rolling
			ret += 4;
			double dw = -t.d*g / J*mintime;
			mw += dw;
			if (mw < 0) mw = 0; //Solvability check

			w.x = mw * cos(beta);
			w.y = mw * sin(beta);
		}

		v = u - a * (k ^ w);

	}
	if (std::abs(r.z) > EPS || std::abs(v.z) > EPS){ //We are in the air
		ret += 8;
		v.z -= G*mintime;

		if (r.z < 0 && v.z < 0){//Floor hit (or underground)
			vec k(0, 0, -1);
			double hi = 2.0 / 5.0;
			double vn = v * k;
			vec u = v - k*(v*k) + a * (w ^ k);

			double vn_n = -t.je * vn;
			double itr_v = t.f * (1 + t.je) * vn;

			vec itr;
			if (u.mod() > itr_v){
				itr = u.normalized() * itr_v;
			}
			else{
				itr = u;
			}

			u -= itr;
			w -= 1 / hi * (k ^ itr);

			v = u - a * (w ^ k) + k * vn_n;
			if (v.z < G*mintime) v.z = 0, ret += 16, r.z = 0;
		}
	}

	r += (mintime * v);
	phi += (quat(0, w) * phi) / 2 * mintime;
	phi = phi.normalized();
	return ret;
};

double Ball::Distance(Ball b)
{
	return sqrt((r.x - b.r.x)*(r.x - b.r.x) + (r.y - b.r.y)*(r.y - b.r.y) + (r.z - b.r.z)*(r.z - b.r.z));
}

