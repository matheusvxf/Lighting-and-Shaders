#include "RobotArm.h"
#include "Cube.h"
#include "MatrixTransform.h"
#include "Robot.h"
#include "Light.h"
#include "Config.h"

RobotArm::RobotArm()
{

}

RobotArm::RobotArm(double arm_width, double arm_height, double arm_depth)
{
    MatrixTransform *m;
    MatrixTransform *light_translate;
    Matrix4 *translate;


    this->arm_width = arm_width;
    this->arm_height = arm_height;
    this->arm_depth = arm_depth;
    this->angle = 0.0;
    this->flag = true;

    model_view = new Matrix4();
    translate = new Matrix4();
    m = new MatrixTransform();
    light_translate = new MatrixTransform();

    this->update_model_view();

    /* Light */
    translate->makeTranslate(0, -arm_height / 2.0, 0);
    light_translate->setTransform(translate);
    light_translate->add_child(new Light());

    m->setTransform(model_view);
    m->add_child(new Cube(1));
    m->add_child(light_translate);

    this->add_child(m);
}

RobotArm::~RobotArm()
{

}

void RobotArm::draw(const Matrix4& C)
{
    Group::draw(C);
}

void RobotArm::update()
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

void RobotArm::update_model_view()
{
    static Matrix4 rotationX, scale, translate, translate_inv;

    translate.makeTranslate(0, -arm_height / 2.0 + arm_width / 2.0, 0);
    rotationX.makeRotateX(angle);
    translate_inv.makeTranslate(0, arm_height / 2.0 - arm_width / 2.0, 0);
    scale.makeScale(arm_width, arm_height, arm_depth);

    *model_view = translate_inv * rotationX * translate * scale;
}