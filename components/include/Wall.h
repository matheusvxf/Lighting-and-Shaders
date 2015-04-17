#pragma once

#include "DataType.h"
#include "Geode.h"

class Wall :
    public Geode
{
protected:
    double height;
    double width;
    UINT32 slices;
    double *vertices;
public:
    Wall(double height, double width, UINT32 slices);
    Wall();
    virtual ~Wall();

    void render();
};

