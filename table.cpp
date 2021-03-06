#include "table.h"

#define isnan(f) (f != f)
#define sign(f) (std::abs(f)<EPS ? 0 : (f>0 ? 1 : -1))
#define max(a,b) (a>b ? a : b)

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

    delete [] dfsed;
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

int Table::NextStep(sndvolume &snd){
	int ret = 0;

    for (auto it = balls.begin(); it != balls.end(); it++){
		if (it -> isvalid == 1){
		    int bc = it -> BoardCollide(*this, snd);
            if (bc & 4){
                ret |= 512;
            }else{
                if (bc & 1)
                    ret |= 1024;
            }
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

        Ball calculus[length];
        do{
            Ball * b = new Ball[length];//Here balls would be copied
            for (auto it = colarr.begin(); it != colarr.end(); ++it)
                b[it-colarr.begin()] = **it;

            for (auto act: collision)
                b[act.first].Collide(*this, b[act.second], snd);

            ctr++;

            for (int i = 0; i < length; i++){
                calculus[i].v += b[i].v;
                calculus[i].w += b[i].w;
            }
            delete [] b;
        }while(std::next_permutation(collision.begin(), collision.end()));

        for (auto it = colarr.begin(); it != colarr.end(); ++it){
            (**it).v = calculus[it-colarr.begin()].v / ctr;
            (**it).w = calculus[it-colarr.begin()].w / ctr;
        }

        ret |= 256;
    }

	for (auto it = balls.begin(); it != balls.end(); it++){
        if (it -> isvalid){
            int retb = it -> NextStep(*this, MINTIME, snd);
            if (it -> isvalid) ret |= (retb);
        }
	}

	return ret;
}

Table::Table(const std::string &name): MINTIME(0), DEFCLOCK(0), MULT(1), FPS(60), SLOWFACTOR(1), sc_b_num(0){
	std::ifstream file(name.c_str());

	if (file.is_open())
	{
        char texture_filename[255];
		file >> bb >> e >> je >> d >> s >> f >> rf >> re >> lenx >> leny >> border_height >> texture_filename;

        if (texture == 0)
            texture = raw_texture_load (texture_filename, 512, 512);
	}
	else
	{
		throw;
	}

	file.close();
};

Table::Table(): MINTIME(0), DEFCLOCK(0), MULT(1), FPS(60), SLOWFACTOR(1), sc_b_num(0){};

Ball::Ball(const  std::string &name, vec r, quat phi, vec v, vec w, const char *texture_filename, float3f track_color) :
r(r),
phi(phi),
v(v),
w(w),
isvalid(true),
track_color(track_color)
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

int Ball::Collide(Table &t, Ball &b, sndvolume &snd)
{

	vec k = (b.r - r).normalized();
	float v1n = v * k;
	float v2n = b.v * k;

    snd.collide = max(snd.collide, std::abs(v2n-v1n));

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
	if ((1 + hi) * 2 * itr_v < mu){ //Always friction
		itr = itr_v * tau;
	}
	else{//Friction stops when u=0
		itr = u / 2 / (1 + hi);
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
    if (Distance(b) > (b.a + a)*(b.a + a)) return 1;//No collide for you

	vec k = (b.r - r).normalized();
    if (isnan(k.mod())) return 2; //Some siht is happening! Let a few step ahead

	float v1n = v * k;
	float v2n = b.v * k;

	if (v2n >= v1n) return 3;//They don't move to each other :(
    return 0;
}

class v6{
public:
    v6(){};
    v6(double a, double b, double c, double p, double q, double r): a(a), b(b), c(c), p(p), q(q), r(r){};
    double a, b, c, p, q, r;

    v6 operator+(v6 op){
        return v6(a+op.a, b+op.b, c+op.c, p+op.p, q+op.q, r+op.r);
    };

    v6 operator*(double op){
        return v6(a*op, b*op, c*op, p*op, q*op, r*op);
    };

    v6 operator/(double op){
        return v6(a/op, b/op, c/op, p/op, q/op, r/op);
    };
};

int Ball::BoardCollide(Table &t, sndvolume &snd){
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
        if (r.z > t.border_height + a) {
            isvalid = 2;
            return 0;
        }

        float hi = 5.0 / 2.0;
		float vn = v * k;
		vec vt = v - k * (v * k);

		vec u = v - k * (v * k) + a * (w ^ k);

        float itr_v = t.rf * (1 + t.re) * vn;

        if (ret & 2) {
            isvalid = 0;
            ret |= 4;
            t.sc_b_num += 1;
            return ret;
        }else{
            snd.board = max(snd.board, std::abs(vn/t.re));
        }

        if (u.z <= 0 || r.z > 0) { //Coriolis
            vec itr;

            if (u.mod() < itr_v*(1+hi)) {
                itr = u/(1+hi);
            }
            else {
                itr = u.normalized() * itr_v;
            }

            vn = -t.re * vn;
            vt -= itr;

            v = vt + vn * k;
            w -= hi * (itr ^ k);
        }else{//Rezal - Beware! Dragons ahead!
            k = vec(0,0,0)-k;//TODO Remove culprit

            vec i(0,0,1);
            vec j = k ^ i;

            u = v + a*(w^k);

            v6 c;
            c.a = u*i;
            c.b = u*j;
            c.c = u*k;
            c.p = w*i;
            c.q = w*j;
            c.r = w*k;

            double magic = 1000; //TODO Magical const
            double mintime = (1 + t.re) * vn / magic;

            auto f = [&t, &hi, this](v6 c){
                v6 ret;

                vec et = vec(0, c.b-c.p*a-c.r*a, c.c+c.q*a).normalized();

                ret.a = -t.rf*hi*c.a/sqrt(c.a*c.a+c.b*c.b)+t.rf*t.f*hi*c.a/sqrt(c.a*c.a+c.b*c.b)*et.z;
                ret.b = -t.rf*(1+hi)*c.b/sqrt(c.a*c.a+c.b*c.b)-t.f*t.rf*c.a/sqrt(c.a*c.a+c.b*c.b)*et.y;
                ret.c = 1-t.f*t.rf*c.a/sqrt(c.a*c.a+c.b*c.b)*et.z;

                ret.p = -hi*t.rf/a*c.b/sqrt(c.a*c.a+c.b*c.b);
                ret.q = -hi*t.rf/a*c.a/sqrt(c.a*c.a+c.b*c.b)-hi*t.f*t.rf/a*c.a/sqrt(c.a*c.a+c.b*c.b)*et.z;
                ret.r = 0-hi*t.rf*t.f/a*c.a/sqrt(c.a*c.a+c.b*c.b)*(c.b-c.p*a-c.r*a);
                return ret;
            };

            for (int i = 0; i < magic; i++){
                v6 k1 = f(c);
                v6 k2 = f(c+k1*mintime/2);
                v6 k3 = f(c+k2*mintime/2);
                v6 k4 = f(c+k3*mintime);

                c = c+(k1+k2*2+k3*2+k4)/6*mintime;
            }

            u = c.a*i+c.b*j+c.c*k;
            w = c.p*i+c.q*j+c.r*k;
            v = u - a*(w^k);
        }
	}
	return ret;
}

int Ball::NextStep(Table &t, float mintime, sndvolume &snd)
{
    int ret = 0;
    if (isvalid == 2){
        goto FLY;
    }
    if (std::abs(r.z) < EPS && std::abs(v.z) < EPS){ //On the cloth
        vec k(0, 0, 1);
        double hi = 2.0 / 5.0;

        vec dw(0, 0, 0);
        vec dv(0, 0, 0);

        if (w.mod() < EPS && v.mod() < EPS) return 0;

        if (std::abs(w.z) > EPS){
            double ddw = -t.s*G/hi/a/a * sign(w.z) * mintime;
            if (sign(w.z) == sign(w.z+ddw)) //ddw can't change direction
                dw += ddw*k, ret |= 1;
        }

        vec omega = w - k*w.z;
        if (omega.mod() > EPS){
            auto f = [&t, &hi, this](vec omega){return -t.d*G/hi/a/a * omega.normalized();};
            vec k1 = f(omega);
            vec k2 = f(omega + k1*mintime/2);
            vec k3 = f(omega + k2*mintime/2);
            vec k4 = f(omega + k3*mintime);

            vec ddo = (k1+2*k2+2*k3+k4)/6*mintime;
            vec omac = omega.normalized() - (omega + ddo).normalized();
            if (omac.mod() < 1) //vector changed it's direction when delta added - it's very near to zero
                dw += ddo, ret |= 2;
        }

        vec u = v + a * (k ^ w);
        if (u.mod() > EPS){
            auto wf = [&t, &hi, &k, this](vec v, vec w){vec u = v + a * (k ^ w); return t.f*G/hi/a * (k ^ u.normalized());};
            auto vf = [&t, &hi, &k, this](vec v, vec w){vec u = v + a * (k ^ w); return -t.f*G*u.normalized();};

            vec wk1 = wf(v, w);
            vec vk1 = vf(v, w);
            vec wk2 = wf(v + vk1*mintime/2, w + wk1*mintime/2);
            vec vk2 = vf(v + vk1*mintime/2, w + wk1*mintime/2);
            vec wk3 = wf(v + vk2*mintime/2, w + wk2*mintime/2);
            vec vk3 = vf(v + vk2*mintime/2, w + wk2*mintime/2);
            vec wk4 = wf(v + vk3*mintime, w + wk3*mintime);
            vec vk4 = vf(v + vk3*mintime, w + wk3*mintime);

            vec ddv = (vk1+2*vk2+2*vk3+vk4)/6*mintime;
            vec ddw = (wk1+2*wk2+2*wk3+wk4)/6*mintime;

            vec ddu = ddv + a * (k ^ ddw);

            vec umac = u.normalized() - (u + ddu).normalized();
            if (umac.mod() < 1) //like previous + w can be zero
                dw += ddw, dv += ddv, ret |= 4;
        }else{//???
            ret |= 4;
        }
        w += dw, v += dv;

        if (!(ret & 1)) w -= w.z * k;
        if (!(ret & 2)) w -= omega;
        if (!(ret & 4)) v = a * (w ^ k);

        goto END_STEP;
    }

    /* Another cases are considered as flying. */
    if (r.z < EPS && v.z < 0){ //Hit the ground
        snd.table = max(snd.table, std::abs(v.z));
        ret |= 8;
        ret |= 16;
        r.z = 0; //Buried is a bad idea
        vec k(0, 0, -1);
		float hi = 5.0 / 2.0;
		float vn = v * k;
		vec u = v - k*(v*k) + a * (w ^ k);

		float vn_n = -t.je * vn;
		float itr_v = t.f * (1 + t.je) * vn;

		vec itr;
		if (u.mod() > itr_v*(1+hi)){
			itr = u.normalized() * itr_v;
		}
		else{
			itr = u/(1+hi);
		}

		u -= itr;
		w -= hi * (k ^ itr);

		v = u - a * (w ^ k) + k * vn_n;
		if (v.z < G*mintime) v.z = 0, ret |= 32, r.z = 0; //Final ground hit

		goto END_STEP;
    }else{//Other variants is considered as free flight. Velocity update is required
    FLY:
        if (r.z < -4.8) isvalid = 0, ret |= 128;
        ret |= 64;
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
	return (r.x - b.r.x)*(r.x - b.r.x) + (r.y - b.r.y)*(r.y - b.r.y) + (r.z - b.r.z)*(r.z - b.r.z);
}

