#pragma once
#include <Entity.h>

class StaticWorldObject :
    public Entity
{
public:
    StaticWorldObject(std::string texture, Transform transform);
    ~StaticWorldObject();

    virtual void Update(double deltaTime);
    virtual void Render();
};

class DynamicWorldObject :
    public Entity
{
public:
    DynamicWorldObject(std::string texture, Transform transform);
    ~DynamicWorldObject();

    virtual void Update(double deltaTime);
    virtual void Render();
};
