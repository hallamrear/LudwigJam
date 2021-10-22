#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "StateDirector.h"
#include "Log.h"
#include "Settings.h"
#include "Time.h"

//todo : this is not good
//todo : abstract renderer
SDL_Renderer* Game::Renderer = nullptr;

Game::Game()
{
	mIsInitialised = false;
	mIsRunning = false;
	mWindow = nullptr;
}

Game::~Game()
{
	if (mIsInitialised)
		Shutdown();
}

void Game::Start()
{
	if(GetIsInitialised())
	{
		SetIsRunning(true);

		const int FPS = 60;
		const int frameDelay = 1000 / FPS;
		Uint32 currentTime = 0, deltaTime = 0, oldTime = 0;
		Uint32 frameTime = 0;
		double DeltaTime = 0.0;
		Time::Get(DeltaTime);

		while(GetIsRunning())
		{
			oldTime = SDL_GetTicks();

			while (GetIsRunning())
			{
				currentTime = SDL_GetTicks();
				deltaTime = currentTime - oldTime;

				DeltaTime = (double)deltaTime / 1000.0;

				if (deltaTime != 0)
				{
					HandleEvents();
					Update(DeltaTime);
					Render();
				}

				frameTime = SDL_GetTicks() - currentTime;
				if (frameDelay > frameTime)
					SDL_Delay(frameDelay - frameTime);

				oldTime = currentTime;
			}
		}
	}
}

void Game::SetFullscreen(SCREEN_STATE state)
{
	switch (state)
	{
	case SCREEN_STATE::WINDOW_FULLSCREEN:
	{
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		auto Width = DM.w;
		auto Height = DM.h;
		SDL_SetWindowSize(mWindow, Width, Height);
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN);
		int w, h;
		SDL_GetWindowSize(mWindow, &w, &h);
		Settings::Get()->SetWindowDimensions(Vector2f((float)w, (float)h));
	}
		break;
	case SCREEN_STATE::WINDOW_BORDERLESS_FULLSCREEN:
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	case SCREEN_STATE::WINDOW_WINDOWED:
		SDL_SetWindowFullscreen(mWindow, 0);
		break;
	}
}

void Game::Initialise(int argc, char* argv[], WindowDetails details)
{
	mIsInitialised = (InitialiseSystems(details) && InitialiseWorldObjects());
	if(mIsInitialised == false)
	{
		Shutdown();
	}
}

bool Game::InitialiseWindow(const char* title, int xpos, int ypos, int width, int height, unsigned int flags, bool isFullscreen)
{
	if (mWindow)
		SDL_DestroyWindow(mWindow);
		
	if (title == "")
		title = "No Window Title Given";

	if (xpos == 0)
		xpos = SDL_WINDOWPOS_CENTERED;

	if (ypos == 0)
		ypos = SDL_WINDOWPOS_CENTERED;

	if (width == 0)
		width = 800;

	if (height == 0)
		height = 600;

	if (isFullscreen)
		flags |= SDL_WINDOW_FULLSCREEN;

	mWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

	if (mWindow)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Window created.");
		return true;
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Window failed to create.");
		return false;
	}
}

bool Game::InitialiseGraphics()
{
	if (!mWindow || mWindow == nullptr)
		return false;

	if (Renderer)
		SDL_DestroyRenderer(Renderer);

	Renderer = SDL_CreateRenderer(mWindow, -1, 0);

	if (Renderer)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Renderer created.");
		return true;
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Renderer failed to create.");
		return false;
	}
}

bool Game::InitialiseWorldObjects()
{
	InputManager::Bind(IM_KEY_CODE::IM_KEY_Z, IM_KEY_STATE::IM_KEY_PRESSED, 
		[] 
		{
			Settings::Get()->SetDrawColliders(!Settings::Get()->GetDrawColliders());
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_X, IM_KEY_STATE::IM_KEY_PRESSED,
		[]
		{
			Settings::Get()->SetDrawLog(!Settings::Get()->GetDrawLog());
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_F1, IM_KEY_STATE::IM_KEY_PRESSED,
		[this]
		{
			SetFullscreen(SCREEN_STATE::WINDOW_WINDOWED);
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_F2, IM_KEY_STATE::IM_KEY_PRESSED,
		[this]
		{
			SetFullscreen(SCREEN_STATE::WINDOW_BORDERLESS_FULLSCREEN);
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_F3, IM_KEY_STATE::IM_KEY_PRESSED,
		[this]
		{
			SetFullscreen(SCREEN_STATE::WINDOW_FULLSCREEN);
		});

	return true;
}

bool Game::InitialiseSystems(WindowDetails details)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Subsystem created.");


		if (InitialiseWindow(details.title.c_str(), details.position.X, details.position.Y, (int)details.dimensions.X, (int)details.dimensions.Y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, false) == false)
			return false;

		Settings::Get()->SetWindowDimensions(details.dimensions);

		if (TTF_Init() < 0)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "Error initializing SDL_ttf");
			Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
		}

		int w, h;
		SDL_GetWindowSize(mWindow, &w, &h);
		Settings::Get()->SetWindowDimensions(Vector2f((float)w, (float)h));

		if (InitialiseGraphics() == false)
			return false;

		SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);

		//Initialise image loader.
		int flags = 0;
		flags |= IMG_INIT_PNG;
		flags |= IMG_INIT_JPG;
		flags |= IMG_INIT_TIF;
		flags |= IMG_INIT_WEBP;

		if (IMG_Init(flags) == 0)
		{
			mIsInitialised = false;
			return false;
		}
			
		return true;
	}
	else
		return false;
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	IMG_Quit();

	mIsInitialised = false;
}

void Game::HandleEvents()
{
	SDL_Event event;
	bool hadWheelEvent = false;
	while (SDL_PollEvent(&event))
	{
		if(!hadWheelEvent)
		{
			if (event.type == SDL_MOUSEWHEEL)
			{
				hadWheelEvent = true;

				if (event.wheel.y > 0)
				{
					Log::LogMessage(LogLevel::LOG_ERROR, "Scroll UP");
					InputManager::Get()->MouseScrollUpdate(IM_SCROLL_DIRECTION::IM_SCROLL_UP);
				}
				else if (event.wheel.y < 0)
				{
					//scroll down
					Log::LogMessage(LogLevel::LOG_ERROR, "Scroll DOWN");
					InputManager::Get()->MouseScrollUpdate(IM_SCROLL_DIRECTION::IM_SCROLL_DOWN);
				}
			}
		}

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:

			if (event.type == SDL_MOUSEBUTTONDOWN)
				InputManager::Get()->MousePressUpdate(event.button.button, true);
			else if (event.type == SDL_MOUSEBUTTONUP)
				InputManager::Get()->MousePressUpdate(event.button.button, false);
				break;

		case SDL_MOUSEMOTION:
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);
			InputManager::Get()->MousePositionUpdate(x, y);
			break;
			
		case SDL_QUIT:
			mIsRunning = false;
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				int w, h;
				SDL_GetWindowSize(mWindow, &w, &h);
				Settings::Get()->SetWindowDimensions(Vector2f((float)w, (float)h));
			}
			break;

		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
				mIsRunning = false;
			else
				InputManager::Get()->KeyPressUpdate(event.key.keysym.sym, true);
			break;

		case SDL_KEYUP:
			InputManager::Get()->KeyPressUpdate(event.key.keysym.sym, false);
			break;

		default:
			break;
		}
	}
}

void Game::Update(double DeltaTime)
{
	InputManager::Update();
	StateDirector::Update(DeltaTime);

	if(Settings::Get()->GetDrawLog())
		Log::Update(DeltaTime);
}

void Game::Render()
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
	SDL_RenderClear(Renderer);
	StateDirector::Render(*Renderer);

	if (Settings::Get()->GetDrawLog())
		Log::Render(*Renderer);

	SDL_RenderPresent(Renderer);
}
