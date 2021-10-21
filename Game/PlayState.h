#pragma once
#include "GameStates.h"
#include <vector>

class Entity;
class Player;
class TextElement;
struct SDL_Renderer;

//todo : remove
class CollisionManifold;

class PlayState :
    public GameState
{
private:
	TextElement* posText;
	TextElement* velText;
	TextElement* accText;
	TextElement* collisionText;

	std::vector<Entity*> entities;
	Player* mPlayer;

public:
	void Start() override;
	void Update(double) override;
	void Render(SDL_Renderer&) override;
	void End() override;
};

