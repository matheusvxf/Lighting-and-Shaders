#include "RobotPart.h"


RobotPart::RobotPart()
{
    this->owner = NULL;
}

RobotPart::~RobotPart() {}

void RobotPart::setOwner(Robot *owner)
{
    this->owner = owner;
}

void RobotPart::update() {}