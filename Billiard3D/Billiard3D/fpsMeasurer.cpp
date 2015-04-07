#include "fpsMeasurer.h"

#define CURTIME std::chrono::system_clock::now()

void fpsMeasure(Table &t){
    //int numballs = t.balls.size();

    /*std::cout << "Start fps measurements!" << std::endl;

    //CLOCK binary search
    int l = 1;
    int r = 999999;

    while (std::abs(l - r) > 10){
        int m = (l+r)/2;
        std::cout << "Trying " << m << " ticks" << std::endl;
        double mt = 1/t.SLOWFACTOR/m;

        std::vector<Ball> balls;

        for(int i = 0; i < numballs; i++){
            balls.push_back(Ball("ball.cfg", vec(rand(), rand(), rand()), quat(1, vec(0, 0, 0)), vec(rand(), rand(), rand()), vec(rand(), rand(), rand())));
        }

        auto start = CURTIME;
        int i;

        for (i = 0; i < m && ((std::chrono::duration<double>)(CURTIME-start)).count() < 1; i++){
            for (Ball& b: balls)
                b.BoardCollide(t);

            for (auto it = balls.begin(); it != balls.end(); ++it)
                for (auto jt = it + 1; jt != balls.end(); ++jt)
                    it->Collide(t, *jt);

            for (Ball& b: balls)
                b.NextStep(t, mt);
        }
        if (i == m){
            l = m;
        }else{
            r = m;
        }
    }*/

    double l = 1;
    double r = 3000000;
    t.CLOCK = (l+r)/2/t.FPS;
    t.MINTIME = 1/t.SLOWFACTOR/t.CLOCK;
}
