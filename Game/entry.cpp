#include "Game.h"

int main(int argc, char* argv[])
{
	Game* game = new Game();;

	game->Initialise(argc, argv);	

	if(game->GetIsInitialised())
	{
		game->Start();
	}

	delete game;
	game = nullptr;

	return 0;
}