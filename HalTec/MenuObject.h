#pragma once
#include "Entity.h"

class MenuObject :
    public Entity
{
public:
    MenuObject(std::string texture_path, Vector2f position, float rotation);
    ~MenuObject() override;

    void			Update(double deltaTime) override;
    void			Render() override;
};

