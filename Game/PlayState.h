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

class PlayState :
    public GameState
{
private:
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
	void PlacePlatforms();
	void Save(std::string location);
	void Load(std::string location);

public:
	void Start() override;
	void Update(double) override;
	void Render(SDL_Renderer&) override;
	void End() override;
};

