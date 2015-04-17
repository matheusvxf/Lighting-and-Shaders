#pragma once

#include "Group.h"
#include "RobotPart.h"
#include "DataType.h"

class RobotArm :
    public RobotPart
{
    friend class Robot;
protected:
    BOOLEAN flag;
private:
    double arm_width;
    double arm_height;
    double arm_depth;
    double angle;
public:
    RobotArm();
    RobotArm(double arm_width, double arm_height, double arm_depth);
    ~RobotArm();

    void draw(const Matrix4& C);
    void update();
    void update_model_view();
};