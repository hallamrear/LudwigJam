#include "Startup.h"
#include "Texture.h"
#include "TextureCache.h"
#include "InputManager.h"
#include "StateDirector.h"
#include "TextElement.h"

void Startup::Start()
{
	mStarted = false;
	mTexture = TextureCache::GetTexture("Textures/help.bmp");
	InputManager::Bind(IM_KEY_CODE::IM_KEY_SPACE, IM_KEY_STATE::IM_KEY_PRESSED, [this] { if (mStarted == false) { mStarted = true; } });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_1, IM_KEY_STATE::IM_KEY_PRESSED, [this] { StateDirector::SetState(GameStateIdentifier::GAME_STATE_1); });

	text = new TextElement(Transform(Vector2f(-400.0f, -400.0f), 0.0f), "GAME HAS NO END WHEN YOU GET TO THE TOP GIVE YOURSELF A PAT ON THE BACK", 20.0F, Colour(255, 255, 255, 255));
}

void Startup::End()
{
	if (text)
	{
		delete text;
		text = nullptr;
	}
}

void Startup::Update(double deltaTime)
{
	if(text)
		text->Update(deltaTime);

	if (mStarted)
		StateDirector::SetState(GameStateIdentifier::GAME_STATE_PLAYING);
}

void Startup::Render(SDL_Renderer& renderer)
{
	if (mTexture)
		mTexture->Render(renderer, Vector2f(0.0f, 0.0f), 0.0f);

	if (text)
		text->Render();
}
