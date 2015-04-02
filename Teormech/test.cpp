#include "vec.h"
#include "table.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "glutRender.h"

#define TIMELOSS 5000

using namespace std;

int main(int argc, char * argv[])
{
    vector<Ball> balls;

    if (argc > 1){
        ifstream file(argv[1]);
        double px, py, pz, rx, ry, rz, vx, vy, vz, wx, wy, wz;
        if (!file.is_open()){
            cout << "Failed to open file!"; return 1;
        }
        while(file >> rx >> ry >> rz >> px >> py >> pz >> vx >> vy >> vz >> wx >> wy >> wz)
            balls.push_back(Ball("ball.cfg", vec(rx, ry, rz), vec(px, py, pz), vec(vx, vy, vz), vec(wx, wy, wz)));
    }else{
        ifstream file("lunev");
        double px, py, pz, rx, ry, rz, vx, vy, vz, wx, wy, wz;
        if (!file.is_open()){
            cout << "Failed to open file!"; return 1;
        }
        while(file >> rx >> ry >> rz >> px >> py >> pz >> vx >> vy >> vz >> wx >> wy >> wz)
            balls.push_back(Ball("ball.cfg", vec(rx, ry, rz), vec(px, py, pz), vec(vx, vy, vz), vec(wx, wy, wz)));
    }

    if (balls.empty()){
        cout << "Больше мячей богу мячей!"; return 1;
    }
	Table t("table.cfg");


    #ifdef DENIS_TEST
    int ret = 0;
    int time = 0;

    cout << balls.size() << endl;
    cout << fixed;
    do{
        time++;
        ret = 0;
        if (time % TIMELOSS == 0) cout << setprecision(5) << time*MINTIME << "\t";

        for(vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it){
            for(vector<Ball>::iterator jt = it+1; jt != balls.end(); ++jt){
                it->Collide(t, (*jt));
            }
        }

        for(vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it){
            int retb = it->NextStep(t);
            if (retb != 1) ret |= (retb);
            if (time % TIMELOSS == 0) cout << setprecision(5) << "\t" << it->r.x << ", " << it->r.y << ", " << it->r.z;
        }
        if (time % TIMELOSS == 0) cout << endl;
    }while(ret);
    #endif // DENIS_TEST

    glutRender::Instance.Init (&argc, argv);

	return 0;

}
