#pragma once
#include "Character.h"

class BackgroundEntity
	: public Character
{
private:
	virtual void SetupInput() {};

public:
	BackgroundEntity(std::string texture, Vector2f position, float rotation);
	~BackgroundEntity();

	virtual void Update(double deltaTime);
	virtual void Render();
};

