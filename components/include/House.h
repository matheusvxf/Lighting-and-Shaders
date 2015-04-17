#pragma once
#include "Object.h"
#include "Camera.h"

class House :
    public Object
{
private:
    Camera camera;
public:
    House();
    ~House();

    void render();
    void setCamera(const Vector4& e, const Vector4& d, const Vector4& up);
};

