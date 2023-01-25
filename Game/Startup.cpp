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

	controlText = new TextElement*[5];
	for (size_t i = 0; i < 3; i++)
	{
		controlText[i] = new TextElement(Transform(Vector2f(0.0f, 0.0f), 0.0f), "", 16.0f, Colour(255, 255, 255, 255));
		controlText[i]->SetString("adfajsdbfnjhabvuiadsv");
	}

	controlText[0]->SetPosition(Vector2f(5.0f, -20.0f));
	controlText[1]->SetPosition(Vector2f(5.0f, 20.0f));
	controlText[2]->SetPosition(Vector2f(5.0f, 60.0f));
	controlText[0]->SetString("Run Right");
	controlText[1]->SetString("Run Left");
	controlText[2]->SetString("Jump");

	text = new TextElement(Transform(Vector2f(0.0f, -100.0f), 0.0f), "", 16.0f, Colour(255, 255, 255, 255));
	text->SetString("GAME HAS NO END WHEN YOU GET TO THE TOP GIVE YOURSELF A PAT ON THE BACK");
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
	for (size_t i = 0; i < 3; i++)
	{
		if (controlText[i])
		{
			controlText[i]->Update(deltaTime);
		}
	}

	if (text)
	{
		//std::string str;
		//str = std::string(std::to_string(InputManager::Get()->GetMouseWorldPosition().X) + ", " + std::to_string(InputManager::Get()->GetMouseWorldPosition().Y));
		//text->SetString(str);
		text->Update(deltaTime);
	}

	if (mStarted)
		StateDirector::SetState(GameStateIdentifier::GAME_STATE_PLAYING);
}

void Startup::Render(SDL_Renderer& renderer)
{
	for (size_t i = 0; i < 3; i++)
	{
		if (controlText[i])
		{
			controlText[i]->Render();
		}
	}

	if (mTexture)
		mTexture->Render(renderer, Vector2f(-100.0f, 0.0f), 0.0f);

	if (text)
		text->Render();
}
