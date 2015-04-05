#include "table.h"

int Table::NextStep(double mintime){
    int ret = 0;

    for(std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it)
        it->BoardCollide(*this);

    for(std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it)
        for(std::vector<Ball>::iterator jt = it+1; jt != balls.end(); ++jt)
            it->Collide(*this, *jt, mintime);

    for(std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it){
        int retb = it->NextStep(*this, mintime);
        if (retb != 1) ret |= (retb);
    }

    return ret;
}

Table::Table(const std::string name){
	std::ifstream file(name.c_str());

	if (file.is_open())
	{
		file >> bb >> e >> je >> d >> s >> f >> rf >> re >> lenx >> leny;
	}
	else
    {
        throw;
	}

	file.close();
};

Table::Table(){};

