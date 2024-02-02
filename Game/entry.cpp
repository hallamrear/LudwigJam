#include <Windows.h>
#include "Game.h"
#include "InputManager.h"
#include "PlayState.h"
#include "GameWonState.h"
#include "Startup.h"
#include "StateDirector.h"
#include "Vector2.h"

void SetupStates()
{
	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_MAIN_MENU, new Startup());
	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_PLAYING, new PlayState());
	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_1, new GameWonState());
	StateDirector::SetState(GameStateIdentifier::GAME_STATE_MAIN_MENU);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	Game* game = new Game();

	WindowDetails details;
	details.title = "Slider";
	details.dimensions = Vector2f(1280.0f, 720.0f);
	details.position = Vector2f(256.0f, 256.0f);

	game->Initialise(0, nullptr, details);

	SetupStates();

	game->Start();

	delete game;
	game = nullptr;
		
	return 0;
}