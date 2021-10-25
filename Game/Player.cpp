#include "Player.h"
#include "Game.h"
#include "InputManager.h"
#include "Log.h"
#include "OrientedBoundingBox.h"
#include "Animation.h"
#include "Vector2.h"
#include "Helper.h"
#include "Time.h"
#include <string>

#include "../HalTec/SDL2/include/SDL.h"
#include <Camera.h>

Player::Player(std::string texture, Transform transform)
	: Character(texture, transform, false)
{
	SetGravityEnabled(true);
	SetDragEnabled(true);

	if(mAnimation)
	{
		delete mAnimation;
		mAnimation = nullptr;
	}

	SetupInput();

	mFacingRight = true;
	mAnimation = new AnimationController("Textures/testSpriteSheet.bmp", 6, 8, 1, true);

	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, (float)mAnimation->FrameSize.X, (float)mAnimation->FrameSize.Y);
}

Player::~Player()
{
	if(mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void Player::SetupInput()
{
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN, IM_KEY_STATE::IM_KEY_PRESSED, [this] { MoveRight(); });
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP, IM_KEY_STATE::IM_KEY_PRESSED, [this] { MoveLeft(); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_D, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveRight(); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_A, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveLeft(); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_W, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveUp(); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_S, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveDown(); });
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_MIDDLE_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this] 
		{ 
			AddForce(mTransform.GetUp() * 50000.0f);
		});
}

void Player::MoveDown()
{
	AddForce(Vector2f(0.0f, -2500.0f));
}

void Player::MoveUp()
{
	AddForce(Vector2f(0.0f, 2500.0f));
}

void Player::MoveRight()
{
	mFacingRight = true;
	AddForce(Vector2f(2500.0f, 0.0f));
}

void Player::MoveLeft()
{
	mFacingRight = false;
	AddForce(Vector2f(-2500.0f, 0.0f));
}

void Player::Update(double deltaTime)
{
	Entity::ClampRotation();
	Entity::UpdatePhysics(deltaTime);

	if (mCollider)
		mCollider->Update(deltaTime);

	if (abs(mVelocity.X) > 0.2f)
	{
		if (mFacingRight)
			mAnimation->SetAnimation(0);
		else
			mAnimation->SetAnimation(2);
	}
	else
		mAnimation->SetAnimation(1);

	if (mAnimation)
		mAnimation->Update(deltaTime);
}

void Player::Render()
{
	Vector2f basis[4] = { mTransform.GetUp(),  mTransform.GetDown(),  mTransform.GetLeft(),  mTransform.GetRight() };
	

	if (mAnimation)
		mAnimation->Render(mRenderer, mTransform.Position, mTransform.Rotation);

	if(mCollider)
		mCollider->Render(mRenderer);


	Vector2f target;
	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			SDL_SetRenderDrawColor(&mRenderer, 255, 0, 0, 255);
			break;
		case 1:
			SDL_SetRenderDrawColor(&mRenderer, 255, 255, 0, 255);
			break;
		case 2:
			SDL_SetRenderDrawColor(&mRenderer, 255, 255, 255, 255);
			break;
		case 3:
			SDL_SetRenderDrawColor(&mRenderer, 255, 0, 255, 255);
			break;
		}

		target = mTransform.Position + (basis[i] * 60);
		Vector2f pos = Camera::WorldToScreen(mTransform.Position);
		target = Camera::WorldToScreen(target);
		SDL_RenderDrawLine(&mRenderer, pos.X, pos.Y, target.X, target.Y);
	}
}