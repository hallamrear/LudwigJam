#pragma once
#include "GameStates.h"
#include "Transform.h"

class BoundingBox;
class Car;
class TextElement;

class AltPlayScene : public GameState
{
private:

public:
	virtual void Start();
	virtual void End();
	virtual void Update(double);
	virtual void Render(SDL_Renderer&);
};
