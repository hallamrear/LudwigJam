#pragma once
#include "GameStates.h"
#include <Transform.h>
#include <vector>

class Entity;
class Player;
class TextElement;
class ForceArea;
struct SDL_Renderer;
class BoundingBox;
class OrientedBoundingBox;

class PlayState :
    public GameState
{
private:
	int buildMode;
	int currentSelected = 1;
	float currentSelectionRotation = 0.0f;
	OrientedBoundingBox* mouseEditCollider;
	Vector2f mousePosition;
	Entity* targetBody;

	std::vector<Entity*> testSelections;

	TextElement* modeString;
	Transform PlayAreaColliderTransform;
	BoundingBox* PlayAreaCollider;
	bool camLerp;

	std::vector<std::pair<int, Entity*>> entities;
	Player* mPlayer;
	TextElement* mousePos;

	ForceArea* fOne;
	ForceArea* fTwo;
	ForceArea* fThree;
	ForceArea* fFour;
	void Save(std::string location);
	void Load(std::string location);

public:
	void Start() override;
	void Update(double) override;
	void Render(SDL_Renderer&) override;
	void End() override;
};

