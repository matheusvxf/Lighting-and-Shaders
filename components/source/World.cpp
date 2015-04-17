#include "main.h"
#include "World.h"
#include "Utility.h"
#include "MatrixTransform.h"
#include "MotionTransform.h"
#include "IUpdatable.h"
#include "Geode.h"
#include "Wall.h"
#include "Switch.h"
#include "Dragon.h"
#include "Bear.h"
#include "Lamp.h"
#include "SpotLight.h"

#include <chrono>
#include <iostream>

static Plane view_frustum[6];

static GLfloat light_position[] = { 1.0, 10.0, 1.0, 0.0 };
static GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat light_diffuse[] = { 0.2, 0.2, 0.2, 1.0 };

double World::near_plane_height, World::near_plane_width;

Plane::Plane() {}

Plane::Plane(const Vector4& normal, const Vector4& center)
{
    this->normal = normal;
    this->center = center;
    this->normal.normalize();
}

World::World()
{
    Matrix4 translate, rotateX, *transformation;
    MatrixTransform *matrix_transform;
    MatrixTransform *mouse_matrix_transform;
    Lamp *lamp;
    Light *light;

    switch_model = new Switch();

    change_scene();
    calculate_frustum();

    switch_model->add_child(new Bunny(), 0x01);
    switch_model->add_child(new Dragon(), 0x02);
    switch_model->add_child(new Bear(), 0x04);
    switch_model->set_mask(0x01);
    add_child(switch_model);

    /* Lights */
    /* Lamp */
    translate.makeTranslate(5, 2, -5);
    transformation = translate.clone();
    matrix_transform = new MatrixTransform(transformation);
    lamp = new Lamp(GL_LIGHT1, 1);
    light = lamp->get_light_pointer();
    light->enable_property(Light::DIFFUSE);
    light->set_property(Light::DIFFUSE, Color(ORANGE));
    light->set_property(Light::SPECULAR, Color(ORANGE));
    matrix_transform->add_child(lamp);
    add_child(matrix_transform);

    /* SpotLight */
    translate.makeTranslate(0.0, 0, 2.0);
    rotateX.makeRotateX(-90);
    transformation = (translate * rotateX).clone();
    mouse_transformation = new Matrix4(Matrix4::MatrixType::tIdentity);
    mouse_matrix_transform = new MatrixTransform(mouse_transformation);
    matrix_transform = new MatrixTransform(transformation);
    mouse_matrix_transform->add_child(matrix_transform);
    spot_light = new SpotLight(GL_LIGHT0, 0.1, 0.1);
    matrix_transform->add_child(spot_light);
    add_child(mouse_matrix_transform);
}

World::~World() {}

void World::init_robot()
{
}

void World::change_scene()
{
    Globals::camera.set_center_of_proj(Vector4(0, 0, 2.0));
    Globals::camera.look_at(Vector4(0, 0, -1));
    Globals::camera.set_up(Vector4(0, 1, 0));

    switch_model->set_mask(Globals::scene);
}

Node *World::get_curr_object()
{
    if (Globals::control_light)
        return spot_light;
    else
        return switch_model->get_first_element();
}

Node *World::get_model()
{
    return switch_model->get_first_element();
}

void World::draw(const Matrix4& C)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT7, GL_POSITION, light_position);
    glLightfv(GL_LIGHT7, GL_SPECULAR, light_ambient);
    glLightfv(GL_LIGHT7, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT7, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHT7);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_COLOR_MATERIAL);

    Group::draw(C);
}

void World::update()
{
    IUpdatable *object;
    auto list = switch_model->get_first_list();
    
    spot_light->update();

    foreach((*list), it)
    {
        object = dynamic_cast<IUpdatable*>(*it);
        
        if (object != nullptr)
            object->update();
    }
}

BOOLEAN World::is_in_view_frostum(const Node& object)
{
    static Vector4 center;
    double distance;
    double distance_module;
    double radius;
    for (register UINT8 i = 0; i < 6; ++i)
    {
        radius = object.get_bounding_sphere_radius();
        center = object.get_bounding_sphere_center();
        distance = distance2plane(view_frustum[i], center);
        distance_module = distance > 0 ? distance : -distance;
        if (distance > radius)
            return FALSE;
    }
    return TRUE;
}

void World::calculate_frustum()
{
    double dx, dy, x, y, z, FOV_w;
    Matrix4 rotX, rotY;
    Vector4 normal;

    near_plane_height = 2.0 * Globals::near_plane * tan(degree2rad(Globals::FOV / 2.0));
    near_plane_width = Globals::aspect * near_plane_height;
    z = -(Globals::near_plane + Globals::far_plane) / 2.0;
    FOV_w = 2 * rad2degree(atan2(near_plane_width / 2.0, Globals::near_plane));
    dy = tan(degree2rad(Globals::FOV / 2.0)) * (-z - Globals::near_plane);
    dx = tan(degree2rad(FOV_w / 2.0)) * (-z - Globals::near_plane);

    /* Near plane */
    view_frustum[0] = Plane(Vector4(0, 0, 1), Vector4(0, 0, -Globals::near_plane));
    /* Far plane */
    view_frustum[1] = Plane(Vector4(0, 0, -1), Vector4(0, 0, -Globals::far_plane));
    /* Right plane */
    rotY.makeRotateY(-FOV_w / 2.0);
    normal.set(1, 0, 0, 0);
    normal = rotY * normal;  
    y = 0;
    x = near_plane_width / 2.0 + dx;
    view_frustum[2] = Plane(normal, Vector4(x, y, z));
    /* Left plane */
    rotY.makeRotateY(FOV_w / 2.0);
    normal.set(-1, 0, 0, 0);
    normal = rotY * normal;
    y = 0;
    x = -x;
    view_frustum[3] = Plane(normal, Vector4(x, y, z));
    /* Up plane */
    y = near_plane_height /2.0 + dy;
    normal.set(0, 1, 0, 0);
    rotX.makeRotateX(Globals::FOV / 2.0);
    normal = rotX * normal;
    view_frustum[4] = Plane(normal, Vector4(0, y, z));
    /* Down plane */
    y = -y;
    normal.set(0, -1, 0, 0);
    rotX.makeRotateX(-Globals::FOV / 2.0);
    normal = rotX * normal;
    view_frustum[5] = Plane(Vector4(normal), Vector4(0, y, z));
}