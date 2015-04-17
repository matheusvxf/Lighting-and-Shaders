#include "RobotTorso.h"
#include "MatrixTransform.h"
#include "Cube.h"
#include "Matrix4.h"

RobotTorso::RobotTorso() : RobotTorso(1.0, 1.0, 1.0) {}

RobotTorso::RobotTorso(double body_width, double body_height, double body_depth)
{
    Matrix4* transformation;
    MatrixTransform* m;

    this->body_width;
    this->body_height;
    this->body_depth;

    transformation = new Matrix4();
    m = new MatrixTransform();
    transformation->makeScale(body_width, body_height, body_depth);
    m->setTransform(transformation);
    m->add_child(new Cube(1));

    this->add_child(m);
}

RobotTorso::~RobotTorso()
{
}
