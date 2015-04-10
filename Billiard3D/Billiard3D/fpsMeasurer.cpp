#include "fpsMeasurer.h"

#define CURTIME std::chrono::system_clock::now()

void fpsTest(std::vector<Ball> &balls, Table &t, double mt){
    for (Ball& b: balls)
        b.BoardCollide(t);

    for (auto it = balls.begin(); it != balls.end(); ++it)
        for (auto jt = it + 1; jt != balls.end(); ++jt)
            it->Collide(t, *jt);

    for (Ball& b: balls)
        b.NextStep(t, mt);
}


int fpsCount(int numballs, Table t){
    std::cout << "Start fps measurements!" << std::endl;

    //CLOCK binary search
    int l = 1;
    int r = 999999;

    while (std::abs(l - r) > 10){
        int m = (l+r)/2;
        std::cout << "Trying " << m << " ticks" << std::endl;
        double mt = 1/t.SLOWFACTOR/m;

        std::vector<Ball> balls;

        for(int i = 0; i < numballs; i++){
            balls.push_back(Ball("ball.cfg", vec(rand(), rand(), rand()), quat(1, vec(0, 0, 0)), vec(rand(), rand(), rand()), vec(rand(), rand(), rand()), ""));
        }

        auto start = CURTIME;
        int i;

        for (i = 0; i < m && ((std::chrono::duration<double>)(CURTIME-start)).count() < 1; i++){
            fpsTest(balls, t, mt);
        }
        if (i == m){
            l = m;
        }else{
            r = m;
        }
    }
    return (l+r)/2;
}

void fpsMeasure(Table &t){
    std::ifstream file("fps.cfg");
    if (!file.is_open())
    {
        std::cout << "No fps file found, measuring..." << std::endl;
        int numballs = t.balls.size();

        t.CLOCK = fpsCount(numballs, t)/t.FPS;
        t.MINTIME = 1/t.SLOWFACTOR/t.CLOCK;

        std::ofstream file_out("fps.cfg");
        file_out << t.CLOCK;
    }else{
        if(!(file >> t.CLOCK)){
            std::cout << "Your fps file is bad, I have to retest" << std::endl;
            int numballs = t.balls.size();
            t.CLOCK = fpsCount(numballs, t)/t.FPS;
            t.MINTIME = 1/t.SLOWFACTOR/t.CLOCK;

            std::ofstream file_out("fps.cfg");
            file_out << t.CLOCK;
        }else{
            int numballs = t.balls.size();
            std::cout << "OK, I found your cfg. Let me check it" << std::endl;

            int m = t.CLOCK * t.FPS;
            std::cout << "Trying " << m << " ticks" << std::endl;
            double mt = 1/t.SLOWFACTOR/m;

            std::vector<Ball> balls;

            for(int i = 0; i < numballs; i++){
                balls.push_back(Ball("ball.cfg", vec(rand(), rand(), rand()), quat(1, vec(0, 0, 0)), vec(rand(), rand(), rand()), vec(rand(), rand(), rand()), ""));
            }

            auto start = CURTIME;
            int i;
            double time;

            for (i = 0; i < m && (time = ((std::chrono::duration<double>)(CURTIME-start)).count()) < 1.15; i++){
                fpsTest(balls, t, mt);
            }

            if (i == m){
                if (time > 0.85){
                    std::cout << "OK, I believe you..." << std::endl;
                    t.MINTIME = 1/t.SLOWFACTOR/t.CLOCK;
                }else{
                    std::cout << "Hmm, your measurements are too inaccurate!" << std::endl;
                    std::cout << "Shall I remeasure it for you? [Y/n] ";

                    char c;
                    std::cin >> c;
                    if (c == 'n' || c == 'N'){
                        std::cout << "As you wish..." << std::endl;
                        t.MINTIME = 1/t.SLOWFACTOR/t.CLOCK;
                        return;
                    }else{
                        std::cout << "Good choice!" << std::endl;
                        int numballs = t.balls.size();
                        t.CLOCK = fpsCount(numballs, t)/t.FPS;
                        t.MINTIME = 1/t.SLOWFACTOR/t.CLOCK;
                        std::ofstream file_out("fps.cfg");
                        file_out << t.CLOCK;
                    }
                }
            }else{
                std::cout << "You lied me! I hate you, have to remeasure it!" << std::endl;
                int numballs = t.balls.size();
                t.CLOCK = fpsCount(numballs, t)/t.FPS;
                t.MINTIME = 1/t.SLOWFACTOR/t.CLOCK;

                std::ofstream file_out("fps.cfg");
                file_out << t.CLOCK;
            }
        }
    }
}
