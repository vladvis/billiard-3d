#include "table.h"

#define isnan(f) (f != f)
#define sign(f) (std::abs(f)<EPS ? 0 : (f>0 ? 1 : -1))

std::vector<std::vector<Ball *>> Table::CollideDFS(){
    int length = balls.size();

    int * dfsed = new int[length];
    for (int i=0; i<length; i++)
        dfsed[i] = 0;
    int color = 1;

    for (auto it = balls.begin(); it != balls.end(); ++it){
        if (!dfsed[it-balls.begin()]){
            dfsed[it-balls.begin()] = color;
            it->CollideDFS(*this, dfsed, color);
            color++;
        }
    }

    std::vector<std::vector<Ball*>> ret;

    for (int i = 1; i < color; i++){
        std::vector<Ball*> newcol;
        for (int j = 0; j < length; j++){
            if (dfsed[j] == i){
                newcol.push_back(&(balls.at(j)));
            }
        }
        if (newcol.size() > 1)
            ret.push_back(newcol);
    }

    delete dfsed;
    return ret;
}

void Ball::CollideDFS(Table &t, int * dfsed, int color){
    for (auto it = t.balls.begin(); it != t.balls.end(); ++it){
        if (!dfsed[it-t.balls.begin()] && !noCollide(*it)){
            dfsed[it-t.balls.begin()] = color;
            it->CollideDFS(t, dfsed, color);
        }
    }
}

int Table::NextStep(){
	int ret = 0;

	//for (std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it)
    for (auto it = balls.begin(); it != balls.end();){
		if (it -> isvalid){
            if (it -> BoardCollide(*this) & 4)
                it = balls.erase(it);
            else
                it++;
		}else{
            it++;
		}
    }

    for (auto colarr: CollideDFS()){
        std::vector<std::pair<int, int>> collision;
        for (auto it = colarr.begin(); it != colarr.end(); ++it){
            for (auto jt = it + 1; jt != colarr.end(); ++jt){
                if (!(*it)->noCollide(**jt))
                    collision.push_back(std::pair<int, int>(it-colarr.begin(), jt-colarr.begin()));
            }
        }

        std::sort(collision.begin(), collision.end());

        double ctr = 0;
        int length = colarr.size();

        Ball * calculus = new Ball[length];
        do{
            Ball * b = new Ball[length];//Here balls would be copied
            for (auto it = colarr.begin(); it != colarr.end(); ++it)
                b[it-colarr.begin()] = **it;

            for (auto act: collision)
                b[act.first].Collide(*this, b[act.second]);

            ctr++;

            for (int i = 0; i < length; i++){
                calculus[i].v += b[i].v;
                calculus[i].w += b[i].w;
            }
            delete b;
        }while(std::next_permutation(collision.begin(), collision.end()));

        for (auto it = colarr.begin(); it != colarr.end(); ++it){
            (**it).v = calculus[it-colarr.begin()].v / ctr;
            (**it).w = calculus[it-colarr.begin()].w / ctr;
        }
        delete calculus;
    }

	for (Ball& b: balls){
        if (b.isvalid){
            int retb = b.NextStep(*this, MINTIME);
            ret |= (retb);
        }
	}

	return ret;
}

Table::Table(const std::string name): MINTIME(0), CLOCK(0), FPS(60), SLOWFACTOR(1), sc_b_num(0){
	std::ifstream file(name.c_str());

	if (file.is_open())
	{
		file >> bb >> e >> je >> d >> s >> f >> rf >> re >> lenx >> leny >> border_height;
	}
	else
	{
		throw;
	}

	file.close();
};

Table::Table(): MINTIME(0), CLOCK(0), FPS(60), SLOWFACTOR(1), sc_b_num(0){};

Ball::Ball(const  std::string name, vec r, quat phi, vec v, vec w, const char *texture_filename) :
r(r),
phi(phi),
v(v),
w(w),
isvalid(true)
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

Ball::Ball() :
r(vec(0,0,0)),
phi(quat(1,vec(0,0,0))),
v(vec(0,0,0)),
w(vec(0,0,0))
{};

int Ball::Collide(Table &t, Ball &b)
{
	vec k = (b.r - r).normalized();
	float v1n = v * k;
	float v2n = b.v * k;

	float hi = 5.0 / 2.0;
	vec v1t = v - v1n * k;
	vec v2t = b.v - v2n * k;

	//Normal components is not affected by friction - only restitution. Adding rand value make mpore real
	float v1n_n = (t.e*(v2n - v1n) + (v1n + v2n)) / 2;// * (1 + ((double)(rand() - RAND_MAX/2) / RAND_MAX) * randmod);
	float v2n_n = (t.e*(v1n - v2n) + (v1n + v2n)) / 2;// * (1 + ((double)(rand() - RAND_MAX/2) / RAND_MAX) * randmod);

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

	return 1;
};

int Ball::noCollide(Ball &b){
    if (Distance(b) > b.a + a) return 1;//No collide for you

	vec k = (b.r - r).normalized();
    if (isnan(k.mod())) return 2; //Some siht is happening! Let a few step ahead

	float v1n = v * k;
	float v2n = b.v * k;

	if (v2n >= v1n) return 3;//They don't move to each other :(
    return 0;
}

int Ball::BoardCollide(Table &t){ //TODO Collision of Rezal
	int ret = 0;
	int state = 0;
	vec k;

    if (t.lenx - r.x < a && v.x > 0)
        k = vec(1, 0, 0), ret |= 1, state=1;

    if (t.lenx + r.x < a && v.x < 0)
        k = vec(-1, 0, 0), ret |= 1, state=2;

    if (t.leny - r.y < a && v.y > 0)
        k = vec(0, 1, 0), ret |= 1, state=3;

    if (t.leny + r.y < a && v.y < 0)
        k = vec(0, -1, 0), ret |= 1, state=4;

    if (t.lenx - r.x < 2*a && v.x > 0 && state != 1)
        ret |= 2;

    if (t.lenx + r.x < 2*a && v.x < 0 && state != 2)
        ret |= 2;

    if (t.leny - r.y < 2*a && v.y > 0 && state != 3)
        ret |= 2;

    if (t.leny + r.y < 2*a && v.y < 0 && state != 4)
        ret |= 2;

    if (std::abs(r.y) < a && state <= 2)
        ret |= 2;

	if (ret & 1){
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

		vn = -t.re * vn;
		vt -= itr;

		v = vt + vn * k;
		w -= 1 / hi * (itr ^ k);

        if (ret & 2)
        {
            ret |= 4;
            t.sc_b_num += 1;
        }
	}
	return ret;
}

int Ball::NextStep(Table &t, float mintime)
{
    int ret = 0;
    if (std::abs(r.z) < EPS && std::abs(v.z) < EPS){ //On the cloth
        vec k(0, 0, 1);
        double hi = 2.0 / 5.0;

        vec dw(0, 0, 0);
        vec dv(0, 0, 0);

        if (w.mod() < EPS && v.mod() < EPS) return 0;

        if (std::abs(w.z) > EPS){
            double ddw = -t.s*G/hi/a/a * sign(w.z) * mintime;
            if (sign(w.z) == sign(w.z+ddw)) //ddw can't change direction
                dw += ddw*k, ret += 1;
        }

        vec omega = w - k*w.z;
        if (omega.mod() > EPS){
            vec ddo = -t.d*G/hi/a/a * omega.normalized()*mintime;
            vec omac = omega.normalized() - (omega + ddo).normalized();
            if (omac.mod() < 1) //vector changed it's direction when delta added - it's very near to zero
                dw += ddo, ret += 2;
        }

        vec u = v + a * (k ^ w);
        if (u.mod() > EPS){
            vec ddw = t.f*G/hi/a * (k ^ u.normalized())*mintime;
            vec ddv = -t.f*G*u.normalized()*mintime;

            vec ddu = ddv + a * (k ^ ddw);

            vec umac = u.normalized() - (u + ddu).normalized();
            if (umac.mod() < 1) //like previous + w can be zero
                dw += ddw, dv += ddv, ret += 4;
        }
        w += dw, v += dv;

        if (!(ret & 1)) w -= w.z * k;
        if (!(ret & 2)) w -= omega;
        if (!(ret & 4)) v = - a * (k ^ w);

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

float Ball::Distance(Ball &b)
{
	return sqrt((r.x - b.r.x)*(r.x - b.r.x) + (r.y - b.r.y)*(r.y - b.r.y) + (r.z - b.r.z)*(r.z - b.r.z));
}

