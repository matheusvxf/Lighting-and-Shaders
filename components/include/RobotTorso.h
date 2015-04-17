#pragma once

#include "Group.h"
#include "RobotPart.h"

class RobotTorso :
    public RobotPart
{
protected:
    double body_width;
    double body_height;
    double body_depth;
public:
    RobotTorso();
    RobotTorso(double body_width, double body_height, double body_depth);
    ~RobotTorso();
};

