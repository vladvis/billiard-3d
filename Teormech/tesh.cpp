#include "vec.h"
#include "table.h"
#include <iostream>

using namespace std;

int main()
{
	Table t("table.cfg");
	Ball b("ball.cfg", vec(0, 0, 0), vec(1, 1, 1), vec(0, 0, 0));


	for(int i=0; i<10000; i++){
		cout << b.r.x << " " << b.r.y << " " << b.r.z << endl;
		b.NextStep(t);
	}
	return 0;
}