#pragma once
#include "Object.h"
#include "Camera.h"
#include "main.h"

class TestObject :
    public Object
{
protected:
        Camera camera;
public:
    TestObject();
    ~TestObject();

    virtual void render();
};

