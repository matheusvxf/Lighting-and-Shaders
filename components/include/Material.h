#pragma once

#include "DataType.h"
#include "Color.h"

class Material
{
public:
    enum tMaterialType
    {
        AMBIENT = 0x01,
        DIFFUSE = 0x02,
        SPECULAR = 0x04,
        EMISSION = 0x08,
        SHININESS = 0x10
    };
protected:
    Color ambient;
    Color diffuse;
    Color specular;
    Color emission;
    float shininess[1];

    tMaterialType mask;
public:

    Material();
    virtual ~Material();

    void enable_property(tMaterialType type);
    void set_property(tMaterialType type, const Color& c);
    void set_property(tMaterialType type, float);
    void unset_material(tMaterialType type);
    BOOLEAN has_material_type(tMaterialType type);

    void render();
    void set_default_material();
};

