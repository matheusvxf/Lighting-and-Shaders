#include "RobotLeg.h"
#include "MatrixTransform.h"
#include "Cube.h"
#include "Robot.h"
#include "Config.h"

RobotLeg::RobotLeg()
{

}

RobotLeg::RobotLeg(double leg_width, double leg_height, double leg_depth)
{
    MatrixTransform *m;

    this->leg_width = leg_width;
    this->leg_height = leg_height;
    this->leg_depth = leg_depth;
    this->angle = 0.0;
    this->flag = true;

    model_view = new Matrix4();
    m = new MatrixTransform();

    this->update_model_view();

    m->setTransform(model_view);
    m->add_child(new Cube(1));
    this->add_child(m);
}

RobotLeg::~RobotLeg()
{

}

void RobotLeg::update()
{
    double speed = owner->getSpeed();
    double angular_speed = speed * 20;

    if (flag)
    {
        angle += angular_speed * dt;
        if (angle >= 30)
            flag = false;
    }
    else
    {
        angle -= angular_speed * dt;
        if (angle <= -30)
            flag = true;
    }

    update_model_view();
}

void RobotLeg::update_model_view()
{
    static Matrix4 rotationX, scale, translate, translate_inv;

    translate.makeTranslate(0, -leg_height / 2.0 + leg_width / 2.0, 0);
    rotationX.makeRotateX(angle);
    translate_inv.makeTranslate(0, leg_height / 2.0 - leg_width / 2.0, 0);
    scale.makeScale(leg_width, leg_height, leg_depth);

    *model_view = translate_inv * rotationX * translate * scale;
}