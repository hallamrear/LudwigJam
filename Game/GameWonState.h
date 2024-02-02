#pragma once
#include <GameStates.h>

class Texture;
class TextElement;

class GameWonState :
    public GameState
{
private:
	Texture* mWonTexture;
	TextElement* mWonText;

public:
	GameWonState();
	~GameWonState();

	void Start() override;
	void Update(double) override;
	void Render(SDL_Renderer&) override;
	void End() override;
};

