#include "table.h"

#define isnan(f) (f != f)
#define sign(f) (std::abs(f)<EPS ? 0 : (f>0 ? 1 : -1))

int Table::NextStep(){
	int ret = 0;

	//for (std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it)
    for (Ball& b: balls)
		b.BoardCollide(*this);

    //std::random_shuffle(balls.begin(), balls.end()); //Not sure if good idea..
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

	float v1n = v * k;
	float v2n = b.v * k;

	if (v2n >= v1n) return 0;//They don't move to each other :(

	float hi = 5.0 / 2.0;
	vec v1t = v - v1n * k;
	vec v2t = b.v - v2n * k;

	//Normal components is not affected by friction - only restitution. Adding rand value make mpore real
	float v1n_n = (t.e*(v2n - v1n) + (v1n + v2n)) / 2 * (1 + ((double)(rand() - RAND_MAX/2) / RAND_MAX) * randmod);
	float v2n_n = (t.e*(v1n - v2n) + (v1n + v2n)) / 2 * (1 + ((double)(rand() - RAND_MAX/2) / RAND_MAX) * randmod);

	vec u = b.v - v + a * ((b.w + w) ^ k) - k * ((b.v - v) * k);
	float mu = u.mod();
	vec tau = u.normalized();

	float itr_v = t.bb * (1 + t.e) * (v1n - v2n) / 2;

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
		float hi = 2.0 / 5.0;
		float vn = v * k;
		vec vt = v - k * (v * k);

		vec u = v - k * (v * k) + a * (w ^ k);
		float itr_v = t.rf * (1 + t.re) * vn;

		vec itr;

		if (u.mod() < itr_v){
			itr = u;
		}
		else{
			itr = u.normalized() * itr_v;
		}

		vn = -t.re * vn * (1 + ((double)(rand() - RAND_MAX/2) / RAND_MAX) * randmod);
		vt -= itr;

		v = vt + vn * k;
		w -= 1 / hi * (itr ^ k);
		ret = 1;
	}
	return ret;
}

int Ball::NextStep(Table t, float mintime)
{
    int ret = 0;
    if (std::abs(r.z) < EPS && std::abs(v.z) < EPS){ //On the cloth
        vec k(0, 0, 1);
        double hi = 2.0 / 5.0;

        vec dw(0, 0, 0);
        vec dv(0, 0, 0);

        if (w.mod() < EPS && v.mod() < EPS) return 0;

        if (std::abs(w.z) > EPS)
            dw -= t.s*G/hi/a/a * sign(w.z) * k;

        vec omega = w - k*w.z;
        if (omega.mod() > EPS)
            dw -= t.d*G/hi/a/a * omega.normalized();

        vec u = v + a * (k ^ w);
        if (u.mod() > EPS){
            dw += t.f*G/hi/a * (k ^ u.normalized());
            dv -= t.f*G*u.normalized();
        }

        dv = dv * mintime; dw = dw * mintime;

        vec wmac = w.normalized() - (w + dw).normalized();
        if (wmac.mod() > 1) //vector changed it's direction when delta added - it's very near to zero
            w = vec(0, 0, 0);
        else
            w += dw, ret += 2;

        vec vmac = v.normalized() - (v + dv).normalized();
        if (vmac.mod() > 1) //vector changed it's direction when delta added - it's very near to zero
            v = vec(0, 0, 0);
        else
            v += dv, ret += 1;

        goto END_STEP;
    }

    /* Another cases are considered as flying. */
    if (r.z < EPS && v.z < 0){ //Hit the ground
        ret += 8;
        r.z = 0; //Buried is a bad idea
        vec k(0, 0, -1);
		float hi = 2.0 / 5.0;
		float vn = v * k;
		vec u = v - k*(v*k) + a * (w ^ k);

		float vn_n = -t.je * vn;
		float itr_v = t.f * (1 + t.je) * vn;

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
		if (v.z < G*mintime) v.z = 0, ret += 4, r.z = 0; //Final ground hit

		goto END_STEP;
    }else{//Other variants is considered as free flight. Speed update is required
        ret += 8;
		v.z -= G*mintime;
		goto END_STEP;
    }

END_STEP:
    r += (mintime * v);
    phi += (quat(0, w) * phi) / 2 * mintime;
    phi = phi.normalized();
    return ret;
}

float Ball::Distance(Ball b)
{
	return sqrt((r.x - b.r.x)*(r.x - b.r.x) + (r.y - b.r.y)*(r.y - b.r.y) + (r.z - b.r.z)*(r.z - b.r.z));
}

