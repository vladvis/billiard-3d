#include "vec.h"
#include "table.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "glutRender.h"

#define KIR_TEST



using namespace std;

int main(int argc, char * argv[])
{
    #ifndef KIR_TEST
    const double MINTIME = 0.000001;
    const int TIMELOSS = 5000;

    Table t("table.cfg");
    int time = 0;

    cout << t.balls.size() << endl;
    cout << fixed;
    while(t.NextStep(MINTIME)){
        time++;
        if (time % TIMELOSS == 0) {
            cout << setprecision(5) << time*MINTIME << "\t";
            for(vector<Ball>::iterator it = t.balls.begin(); it != t.balls.end(); ++it)
                cout << setprecision(5) << "\t" << it->r.x << ", " << it->r.y << ", " << it->r.z;
            cout << endl;
        }
    }
	#else
	glutRender::Instance.Init(&argc, argv, "table.cfg", "ball.cfg");
	#endif
	return 0;

}
