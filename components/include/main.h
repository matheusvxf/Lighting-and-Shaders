#ifndef _MAIN_H_
#define _MAIN_H_

#include "GLee.h"
#include "Camera.h"
#include "Bunny.h"
#include "Cube.h"
#include "House.h"
#include "Robot.h"
#include "World.h"

enum tScene
{
    tBunny = 0x01,
    tDragon = 0x02,
    tBear = 0x04
};

namespace Globals
{
    extern Camera camera;
    extern World* world;
    extern Object **curr_obj;
    extern GLuint texture[128];
    extern BOOLEAN draw_bounding_sphere;
    extern BOOLEAN control_light;
    extern BOOLEAN per_pixel_lighting;
    extern BOOLEAN culling;
    extern double FOV;
    extern double near_plane;
    extern double far_plane;
    extern double aspect;
    extern tScene scene;
    extern GLuint vertexShaderObject, fragmentShaderObject;
};

#endif