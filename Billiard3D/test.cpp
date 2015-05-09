#include "vec.h"
#include "table.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <pthread.h>
#include "glutRender.h"



using namespace std;

int main(int argc, char * argv[])
{
	glutRender::Instance.Init(&argc, argv, "table.cfg", "ball.cfg", "sounds.cfg");
}
