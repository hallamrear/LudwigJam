#pragma once
#include "Character.h"

class BackgroundEntity
	: public Character
{
private:
	virtual void SetupInput() {};

public:
	BackgroundEntity(std::string texture, Transform transform);
	~BackgroundEntity();

	virtual void Update(double deltaTime);
	virtual void Render();
};

