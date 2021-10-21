#pragma once
#include "Entity.h"
class Animation;

class Character :
    public Entity
{
protected:
    Animation* mAnimation;

public:
					Character(std::string texture, Vector2f position, float rotation);
    virtual         ~Character();
    virtual void    SetupInput() = 0;
    virtual void    Update(double deltaTime) = 0;
    virtual void	Render() = 0;
};

