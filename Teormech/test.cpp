#include "vec.h"
#include "table.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char * argv[])
{
    /*
    double vx, vy, vz, wx, wy, wz;

    if (argc > 1){
        ifstream file(argv[1]);
        if (!file.is_open()){
            cout << "Failed to open file!"; return 1;
        }
        file >> vx >> vy >> vz >> wx >> wy >> wz;
    }else{
        ifstream file("data");
        if (!file.is_open()){
            cout << "Failed to open file!"; return 1;
        }
        file >> vx >> vy >> vz >> wx >> wy >> wz;
    }

	Table t("table.cfg");
	Ball b("ball.cfg", vec(0, 0, 0), vec(vx, vy, vz), vec(wx, wy, wz));

    //long long int second= 1/MINTIME;

    int ret;
    while((ret = b.NextStep(t))){
	//for(long long int i=0; i<10/MINTIME; i++){
	    //if (!(i % 10))
	    cout << ret << ",";
        cout << b.r.x << "," << b.r.y << "," << b.r.z << ",";
        cout << b.v.x << "," << b.v.y << "," << b.v.z << ",";
        cout << b.w.x << "," << b.w.y << "," << b.w.z << endl;

		//b.NextStep(t);
	//}
    }
    */

    Table t("table.cfg");
	Ball b1("ball.cfg", vec(0, 0, 0), vec(1, 0, 3), vec(0, 0, 0));
	Ball b2("ball.cfg", vec(0.01, 0, 0), vec(-1, 0, -3), vec(0, 0, 0));

    b1.Collide(t, b2);

    cout << b1.v.x << " " << b1.v.y << " " << b1.v.z << endl;
    cout << b2.v.x << " " << b2.v.y << " " << b2.v.z << endl;

	return 0;

}
