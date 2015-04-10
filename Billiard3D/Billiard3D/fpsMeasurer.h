#ifndef FPSMEASURER_TABLE
#define FPSMEASURER_TABLE

#include "table.h"
#include <chrono>
#include <vector>
#include <iostream>

class fpsMeasurer{
    Table &t;

public:
    fpsMeasurer(Table& t);
    void fpsMeasure();

private:
    int fpsCount(int numballs);
    void fpsTest(std::vector<Ball> &balls, double mt);
};

#endif
