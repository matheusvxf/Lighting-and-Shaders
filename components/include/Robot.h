#pragma once

#include "RenderableObject.h"
#include "IUpdatable.h"
#include "RobotPart.h"

#include <list>

class Robot :
    public RenderableObject
{
    std::list<RobotPart*> body_parts;
    Matrix4 *robot_model_view;
    BOOLEAN is_visible;
public:
    Robot();
    virtual ~Robot();

    void Robot::draw(const Matrix4& C);
    void update();
};

