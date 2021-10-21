#pragma once
#include <Entity.h>

class WorldObject :
    public Entity
{
public:
    WorldObject(std::string texture, Vector2f position, float rotation);
    ~WorldObject();

    virtual void Update(double deltaTime);
    virtual void Render();
};

