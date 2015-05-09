#include "vec.h"
#include "glutRender.h"

using namespace std;

int main(int argc, char * argv[])
{
	glutRender::Instance.Init(&argc, argv, "table.cfg", "ball.cfg", "sounds.cfg");
	return 0;
}
