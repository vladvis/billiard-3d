#include "vec.h"
#include "table.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argv, char * argc[])
{
    double vx, vy, vz, wx, wy, wz;

    if (argv > 1){
        ifstream file(argc[1]);
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
	return 0;
}
