#pragma once
#include "Group.h"
#include "RobotPart.h"

class RobotHead :
    public RobotPart
{
private:
    double radius;
public:
    RobotHead();
    RobotHead(double);
    ~RobotHead();
};

