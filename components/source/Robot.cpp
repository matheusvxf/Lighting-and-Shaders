#include "main.h"
#include "Robot.h"
#include "RobotArm.h"
#include "RobotLeg.h"
#include "RobotHead.h"
#include "RobotTorso.h"
#include "Cube.h"
#include "Sphere.h"
#include "MatrixTransform.h"
#include "Utility.h"

#include <algorithm>

static double torso_height = 2;
static double torso_width = 1;
static double torso_depth = 0.5;
static double arm_height = 1.0;
static double arm_width = 0.25;
static double arm_depth = 0.4;
static double leg_height = 1.30;
static double leg_width = 0.3;
static double leg_depth = 0.4;
static double head_radius = 0.5;

Robot::Robot()
{
    MatrixTransform *m, *model_view;
    Matrix4 *translate, *rotateZ, *transform;
    Vector4 p, p_prime, p_dprime, translation;
    RobotArm *right_arm, *left_arm;
    RobotLeg *right_leg, *left_leg;
    RobotHead *head;
    RobotTorso *torso;
    double radius;

    this->robot_model_view = new Matrix4();
    model_view = new MatrixTransform();
    model_view->setTransform(this->robot_model_view);
    this->add_child(model_view);

    /* Torso */
    torso = new RobotTorso(torso_width, torso_height, torso_depth);
    model_view->add_child(torso);

    /* Head */
    head = new RobotHead(head_radius);
    m = new MatrixTransform();
    translate = new Matrix4();
    translate->makeTranslate(0.0, head_radius / 2.0 + torso_height / 2.0, 0.0);
    m->setTransform(translate);
    m->add_child(head);
    model_view->add_child(m);

    /* Right arm */
    right_arm = new RobotArm(arm_width, arm_height, arm_depth);
    rotateZ = new Matrix4();
    translate = new Matrix4();
    m = new MatrixTransform();
    rotateZ->makeRotateZ(45.0);
    p[0] = arm_width / 2.0;
    p[1] = arm_height / 2.0;
    p[2] = 0.0;
    p[3] = 1.0;
    p_prime = *rotateZ * p;
    p_dprime[0] = torso_width / 2.0;
    p_dprime[1] = torso_height / 2.0;
    p_dprime[2] = 0;
    p_dprime[3] = 1;
    translation = p_dprime - p_prime;
    translate->makeTranslate(translation[0], translation[1], 0);
    transform = (*translate * *rotateZ).clone();
    m->setTransform(transform);
    m->add_child(right_arm);
    model_view->add_child(m);

    delete(rotateZ);
    delete(translate);

    /* Left arm */
    left_arm = new RobotArm(arm_width, arm_height, arm_depth);
    rotateZ = new Matrix4();
    translate = new Matrix4();
    m = new MatrixTransform();
    rotateZ->makeRotateZ(-45.0);
    p[0] = -arm_width / 2.0;
    p[1] = arm_height / 2.0;
    p[2] = 0.0;
    p[3] = 1.0;
    p_prime = *rotateZ * p;
    p_dprime[0] = -torso_width / 2.0;
    p_dprime[1] = torso_height / 2.0;
    p_dprime[2] = 0;
    p_dprime[3] = 1;
    translation = p_dprime - p_prime;
    translate->makeTranslate(translation[0], translation[1], 0);
    transform = (*translate * *rotateZ).clone();
    m->setTransform(transform);
    m->add_child(left_arm);
    model_view->add_child(m);

    delete(rotateZ);
    delete(translate);

    /* Left Leg */
    left_leg = new RobotLeg(leg_width, leg_height, leg_depth);
    translate = new Matrix4();
    m = new MatrixTransform();
    p[0] = -leg_width / 2.0;
    p[1] = leg_height / 2.0;
    p[2] = 0;
    p[3] = 1.0;
    p_prime[0] = -torso_width / 2.0;
    p_prime[1] = -torso_height / 2.0;
    p_prime[2] = 0;
    p_prime[3] = 1.0;
    translation = p_prime - p;
    translate->makeTranslate(translation[0], translation[1], 0);
    transform = (*translate).clone();
    m->setTransform(transform);
    m->add_child(left_leg);
    model_view->add_child(m);

    delete(translate);

    /* Right Leg */
    right_leg = new RobotLeg(leg_width, leg_height, leg_depth);
    translate = new Matrix4();
    m = new MatrixTransform();
    p[0] = leg_width / 2.0;
    p[1] = leg_height / 2.0;
    p[2] = 0;
    p[3] = 1.0;
    p_prime[0] = torso_width / 2.0;
    p_prime[1] = -torso_height / 2.0;
    p_prime[2] = 0;
    p_prime[3] = 1.0;
    translation = p_prime - p;
    translate->makeTranslate(translation[0], translation[1], 0);
    transform = (*translate).clone();
    m->setTransform(transform);
    m->add_child(right_leg);
    model_view->add_child(m);

    delete(translate);

    /* Add body parts */
    body_parts.push_back(head);
    body_parts.push_back(left_arm);
    body_parts.push_back(right_arm);
    body_parts.push_back(left_leg);
    body_parts.push_back(right_leg);
    body_parts.push_back(torso);

    foreach(body_parts, part)
    {
        (*part)->setOwner(this);
    }
    left_arm->flag = false;
    right_leg->flag = false;

    radius = (torso_height + head_radius * 2.0 + leg_height) / 2.0;
    this->set_bouding_box(Vector3(radius, 0, 0), Vector3(0, radius, 0), Vector3(0, 0, radius));
}

Robot::~Robot()
{
    body_parts.clear();
}

void Robot::draw(const Matrix4& C)
{
    Vector4 v;
    double max = -1;
    Matrix4 transformation = Globals::camera.getMatrix() * C * model2world;

    for (register UINT8 i = 0; i < 3; ++i)
    {
        v = transformation * bounding_box[i];
        max = max(max, v.length_sq());
    }

    this->bounding_sphere_center[0] = transformation[0][3];
    this->bounding_sphere_center[1] = transformation[1][3];
    this->bounding_sphere_center[2] = transformation[2][3];
    this->bounding_sphere_radius = sqrt(max);

    if (Globals::draw_bounding_sphere)
        this->draw_bounding_sphere();

    if (Globals::culling)
        is_visible = World::is_in_view_frostum(*this);
    else
        is_visible = true;

    if (is_visible)
        Group::draw(C);
}

void Robot::update()
{
    Object::update();
    *this->robot_model_view = this->model2world;

    foreach(body_parts, part)
    {
        (*part)->update();
    }
}