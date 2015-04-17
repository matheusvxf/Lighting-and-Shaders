#include "House.h"
#include "main.h"

#include <GL/glut.h>

// This data structure defines a simple house

// This data structure defines a simple house

int nVerts = 42;    // your vertex array needs to have this many entries

// These are the x,y,z coordinates of the vertices of the triangles
float vertices[] = {
    -4, -4, 4, 4, -4, 4, 4, 4, 4, -4, 4, 4,     // front face
    -4, -4, -4, -4, -4, 4, -4, 4, 4, -4, 4, -4, // left face
    4, -4, -4, -4, -4, -4, -4, 4, -4, 4, 4, -4,  // back face
    4, -4, 4, 4, -4, -4, 4, 4, -4, 4, 4, 4,     // right face
    4, 4, 4, 4, 4, -4, -4, 4, -4, -4, 4, 4,     // top face
    -4, -4, 4, -4, -4, -4, 4, -4, -4, 4, -4, 4, // bottom face

    -20, -4, 20, 20, -4, 20, 20, -4, -20, -20, -4, -20, // grass
    -4, 4, 4, 4, 4, 4, 0, 8, 4,                       // front attic wall
    4, 4, 4, 4, 4, -4, 0, 8, -4, 0, 8, 4,               // left slope
    -4, 4, 4, 0, 8, 4, 0, 8, -4, -4, 4, -4,             // right slope
    4, 4, -4, -4, 4, -4, 0, 8, -4 };                   // rear attic wall

// These are the RGB colors corresponding to the vertices, in the same order
float colors[] = {
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,  // front is red
    0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,  // left is green
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,  // back is red
    0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,  // right is green
    0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,  // top is blue
    0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,  // bottom is blue

    0, 0.5, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0.5, 0, // grass is dark green
    0, 0, 1, 0, 0, 1, 0, 0, 1,                // front attic wall is blue
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,         // left slope is green
    0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,         // right slope is red
    0, 0, 1, 0, 0, 1, 0, 0, 1, };              // rear attic wall is red

// The index data stores the connectivity of the triangles; 
// index 0 refers to the first triangle defined above
int indices[] = {
    0, 2, 3, 0, 1, 2,      // front face
    4, 6, 7, 4, 5, 6,      // left face
    8, 10, 11, 8, 9, 10,     // back face
    12, 14, 15, 12, 13, 14,   // right face
    16, 18, 19, 16, 17, 18,   // top face
    20, 22, 23, 20, 21, 22,   // bottom face

    24, 26, 27, 24, 25, 26,   // grass
    28, 29, 30,             // front attic wall
    31, 33, 34, 31, 32, 33,   // left slope
    35, 37, 38, 35, 36, 37,   // right slope
    39, 40, 41 };            // rear attic wall

House::House()
{
    obj_type = Object::ObjectType::tHouse;
    camera.set_center_of_proj(0, 10, 10);
    camera.look_at(0, 0, 0);
    camera.set_up(0, 1, 0);
}

House::~House()
{
}

void House::setCamera(const Vector4& e, const Vector4& d, const Vector4& up)
{
    camera.set_center_of_proj(e);
    camera.look_at(d);
    camera.set_up(up);
}

void House::render()
{
    Object::render();
    
    /* Set camera projection */
    Globals::camera = this->camera;

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_LIGHTING);

    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    
    glBegin(GL_TRIANGLES);
    for (UINT32 i = 0; i < sizeof(indices) / sizeof(int); ++i)
    {
        glArrayElement(indices[i]);
    }
    glEnd();

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_LIGHTING);
}