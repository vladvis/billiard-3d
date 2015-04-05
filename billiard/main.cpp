#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

#define KIR_TEST

#ifndef KIR_TEST
#include "math/vec.h"
#include "physics/table.h"
#else
#include "graphics/glutRender.h"
#endif // KIR_TEST

using namespace std;

int main(int argc, char * argv[])
{
    #ifndef KIR_TEST
    const double MINTIME = 0.000001;
    const int TIMELOSS = 5000;

    Table GameTable("table.cfg");
    int time = 0;

    std::ifstream file(argc > 1 ? argv[1]: "data");
    double px, py, pz, rx, ry, rz, vx, vy, vz, wx, wy, wz;
    if (!file.is_open())
    {
        std::cout << "Failed to open file!";
        return 1;
    }
    while(file >> rx >> ry >> rz >> px >> py >> pz >> vx >> vy >> vz >> wx >> wy >> wz)
        GameTable.balls.push_back(Ball("ball.cfg", vec(rx, ry, rz), vec(px, py, pz), vec(vx, vy, vz), vec(wx, wy, wz)));

    if (GameTable.balls.empty()){
        std::cout << "Больше мячей богу мячей!";
        return 1;
    }

    cout << GameTable.balls.size() << endl;
    cout << fixed;
    while(GameTable.NextStep(MINTIME)){
        time++;
        if (time % TIMELOSS == 0) {
            cout << setprecision(5) << time*MINTIME << "\t";
            for(vector<Ball>::iterator it = GameTable.balls.begin(); it != GameTable.balls.end(); ++it)
                cout << setprecision(5) << "\t" << it->r.x << ", " << it->r.y << ", " << it->r.z;
            cout << endl;
        }
    }
	#else
	glutRender::Instance.Init(&argc, argv, "table.cfg", "ball.cfg");
	#endif
	return 0;

}
