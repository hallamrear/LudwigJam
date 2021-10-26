#pragma once
#include "Entity.h"

class AnimationController;

class BackgroundEntity
	: public Entity
{
private:
	AnimationController* mAnimation;

public:
	BackgroundEntity(std::string texture, Transform transform);
	~BackgroundEntity();

	virtual void Update(double deltaTime);
	virtual void Render();
};

