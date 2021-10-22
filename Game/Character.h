#pragma once
#include "Entity.h"
class AnimationController;

class Character :
    public Entity
{
protected:
    AnimationController* mAnimation;

public:
					Character(std::string texture, Transform transform, bool isStatic);
    virtual         ~Character();
    virtual void    SetupInput() = 0;
    virtual void    Update(double deltaTime) = 0;
    virtual void	Render() = 0;
};

