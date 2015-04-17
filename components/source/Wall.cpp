#include "Wall.h"

#include <algorithm>

Wall::Wall(double height, double width, UINT32 slices) : height(height), width(width), slices(slices)
{
    double width_slice = width / (double)slices;
    double height_slice = height / (double)slices;
    double x_offset, y_offset;
    double radius;
    vertices = new double[slices * slices * 12];

    x_offset = y_offset = 0;

    for (UINT32 i = 0; i < slices; ++i)
    {
        y_offset = height / 2.0 - i * height_slice;
        for (UINT32 j = 0; j < slices; ++j)
        {
            x_offset = -width / 2.0 + j * width_slice;
            vertices[i * slices * 12 + j * 12 + 0] = x_offset;
            vertices[i * slices * 12 + j * 12 + 1] = y_offset;
            vertices[i * slices * 12 + j * 12 + 2] = 0;
            vertices[i * slices * 12 + j * 12 + 3] = x_offset + width_slice;
            vertices[i * slices * 12 + j * 12 + 4] = y_offset;
            vertices[i * slices * 12 + j * 12 + 5] = 0;
            vertices[i * slices * 12 + j * 12 + 6] = x_offset + width_slice;
            vertices[i * slices * 12 + j * 12 + 7] = y_offset - height_slice;
            vertices[i * slices * 12 + j * 12 + 8] = 0;
            vertices[i * slices * 12 + j * 12 + 9] = x_offset;
            vertices[i * slices * 12 + j * 12 + 10] = y_offset - height_slice;
            vertices[i * slices * 12 + j * 12 + 11] = 0;
        }
    }

    radius = sqrt(height * height + width * width);
    this->set_bouding_box(Vector3(radius, 0, 0), Vector3(0, radius, 0), Vector3(0, 0, radius));
}

Wall::Wall() : Wall(1, 1, 1) {}

Wall::~Wall()
{
    delete []vertices;
}

void Wall::render()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, vertices);
    glNormal3d(0, 0, 1);
    glDrawArrays(GL_QUADS, 0, slices * slices * 4);
}