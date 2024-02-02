#include "GameWonState.h"
#include "Texture.h"
#include "TextureCache.h"
#include "TextElement.h"
#include "Vector2.h"
#include "Camera.h"

GameWonState::GameWonState()
{
	mWonTexture = TextureCache::GetTexture("Textures/congrats.bmp");

	mWonText = new TextElement(
		Transform(Vector2f(0.0f, -20.0f), 0.0f),
		"Thanks for playing!",
		16.0f,
		Colour(255, 255, 255, 255));
}

GameWonState::~GameWonState()
{
	if (mWonText)
	{
		delete mWonText;
		mWonText = nullptr;
	}

	mWonTexture = nullptr;
}

void GameWonState::Start()
{
	Camera::SetCameraPosition(Vector2f(0.0f, 0.0f));
}

void GameWonState::Update(double deltaTime)
{
	if (mWonText)
	{
		mWonText->Update(deltaTime);
	}
}

void GameWonState::Render(SDL_Renderer& renderer)
{
	if (mWonTexture)
	{
		Vector2f position = Vector2f(0.0f, 20.0f);
		float rotation = 0.0f;
		mWonTexture->Render(renderer, position, rotation);
	}

	if (mWonText)
	{
		mWonText->Render();
	}
}

void GameWonState::End()
{
	
}
