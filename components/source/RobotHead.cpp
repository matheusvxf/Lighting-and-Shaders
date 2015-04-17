#include "RobotHead.h"
#include "MatrixTransform.h"
#include "Sphere.h"

RobotHead::RobotHead()
{
}

RobotHead::RobotHead(double radius)
{
    this->radius = radius;

    this->add_child(new Sphere(radius));
}

RobotHead::~RobotHead()
{
}
