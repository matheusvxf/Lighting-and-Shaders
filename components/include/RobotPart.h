#pragma once
#include "Group.h"
#include "Matrix4.h"

class Robot;

class RobotPart :
    public Group
{
    friend class Robot;
protected:
    Robot* owner;
    Matrix4* model_view;
public:
    RobotPart();
    virtual ~RobotPart() = 0;

    void setOwner(Robot *owner);
    
    virtual void update();
};

