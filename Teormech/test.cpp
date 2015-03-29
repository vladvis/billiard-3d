#include "vec.h"
#include "table.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argv, char * argc[])
{
    ifstream file(argc[1]);

    double vx, vy, vz, wx, wy, wz;
    file >> vx >> vy >> vz >> wx >> wy >> wz;

	Table t("table.cfg");
	Ball b("ball.cfg", vec(0, 0, 0), vec(vx, vy, vz), vec(wx, wy, wz));

    long long int second= 1/MINTIME;

	for(long long int i=0; i<10/MINTIME; i++){
	    //if (!(i % 10))
        cout << b.r.x << "," << b.r.y << "," << b.r.z << endl;
		b.NextStep(t);
	}
	return 0;
}
