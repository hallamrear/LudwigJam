#include "Startup.h"
#include "Texture.h"
#include "TextureCache.h"
#include "InputManager.h"
#include "StateDirector.h"
#include "TextElement.h"

Startup::Startup()
{
	mStarted = false;
	mControlTexture = nullptr;
	for (size_t i = 0; i < 4; i++)
	{
		mControlText[i] = nullptr;
	}
}

Startup::~Startup()
{
}

void Startup::Start()
{
	mControlTexture = TextureCache::GetTexture("Textures/help.bmp");
	InputManager::Bind(IM_KEY_CODE::IM_KEY_SPACE, IM_KEY_STATE::IM_KEY_PRESSED, [this] { if (mStarted == false) { mStarted = true; } });

	for (size_t i = 0; i < 4; i++)
	{
		mControlText[i] = new TextElement(Transform(), "", 16.0f);
		mControlText[i]->SetColour(Colour(255, 255, 255, 255));
		mControlText[i]->SetString("adfajsdbfnjhabvuiadsv");
	}

	mControlText[0]->SetPosition(Vector2f(5.0f, -20.0f));
	mControlText[0]->SetString("Run Right");
	mControlText[1]->SetPosition(Vector2f(5.0f, 20.0f));
	mControlText[1]->SetString("Run Left");
	mControlText[2]->SetPosition(Vector2f(5.0f, 60.0f));
	mControlText[2]->SetString("Jump");
	mControlText[3]->SetPosition(Vector2f(0.0f, -100.0f));
	mControlText[3]->SetString("Traverse your way through the level to reach the coin at the end. Press space to start.");
}

void Startup::End()
{
	
}

void Startup::Update(double deltaTime)
{
	for (size_t i = 0; i < 4; i++)
	{
		if(mControlText[i])
			mControlText[i]->Update(deltaTime);
	}

	if (mStarted)
		StateDirector::SetState(GameStateIdentifier::GAME_STATE_PLAYING);
}

void Startup::Render(SDL_Renderer& renderer)
{
	for (size_t i = 0; i < 4; i++)
	{
		if (mControlText[i])
			mControlText[i]->Render();
	}

	if (mControlTexture)
		mControlTexture->Render(renderer, Vector2f(-100.0f, 0.0f), 0.0f);
}
