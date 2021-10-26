#pragma once
#include <Rigidbody.h>

class StaticWorldObject :
    public Rigidbody
{
public:
    StaticWorldObject(std::string texture, Transform transform);
    ~StaticWorldObject();

    virtual void Update(double deltaTime);
    virtual void Render();
};

class DynamicWorldObject :
    public Rigidbody
{
public:
    DynamicWorldObject(std::string texture, Transform transform, PhysicsProperties properties);
    ~DynamicWorldObject();

    virtual void Update(double deltaTime);
    virtual void Render();
};
