#include "vec.h"
#include "table.h"
#include <iostream>

using namespace std;

int main()
{
	Table t("table.cfg");
	Ball b("ball.cfg", vec(0, 0, 0), vec(1, 1, 0), vec(1, 0, 1));


	for(int i=0; i<60/MINTIME; i++){
		cout << b.r.x << " " << b.r.y << " " << b.r.z << " " << b.v.x << " " << b.v.y << " " << b.v.z << endl;
		b.NextStep(t);
	}
	return 0;
}
