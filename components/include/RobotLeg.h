#pragma once

#include "Group.h"
#include "RobotPart.h"
#include "DataType.h"

class RobotLeg :
    public RobotPart
{
    friend class Robot;
private:
    double leg_width;
    double leg_height;
    double leg_depth;
    double angle;
    BOOLEAN flag;
public:
    RobotLeg();
    RobotLeg(double leg_width, double leg_height, double leg_depth);
    ~RobotLeg();

    void update();
    void update_model_view();
};