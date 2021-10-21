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

Player::Player(std::string texture, Vector2f position, float rotation)
	: Character(texture, position, rotation)
{
	SetPhysicsEnabled(true);
	SetGravityEnabled(false);
	SetDragEnabled(true);

	if(mAnimation)
	{
		delete mAnimation;
		mAnimation = nullptr;
	}

	SetupInput();

	mFacingRight = true;
	mRunningRight = new Animation("Textures/RunningRight.bmp", 8, 1, true);
	mRunningLeft = new Animation("Textures/RunningLeft.bmp", 8, 1, true);

	mCollider = new OrientedBoundingBox(mPosition, mRotation, (float)mRunningRight->FrameWidth, (float)mRunningRight->FrameHeight);
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
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP, IM_KEY_STATE::IM_KEY_PRESSED, [this] { mRotation -= (200.0f * Time::DeltaTime()); });
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN, IM_KEY_STATE::IM_KEY_PRESSED, [this] { mRotation += (200.0f * Time::DeltaTime()); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_D, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveRight(); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_A, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveLeft(); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_W, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveUp(); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_S, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveDown(); });
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_MIDDLE_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this] 
		{ 
			/*float angle = ConvertToRadians(mRotation - 90.0f);
			Vector2f up;
			up.X = cos(angle);
			up.Y = sin(angle);
			AddForce(up.GetNormalized() * 200.0f);*/

			AddForce(Vector2f(0.0f, 1.0f) * 500.0f);
		});
}

void Player::MoveDown()
{
	AddForce(Vector2f(0.0f, -250.0f * Time::DeltaTime()));
}

void Player::MoveUp()
{
	AddForce(Vector2f(0.0f, 250.0f * Time::DeltaTime()));
}

void Player::MoveRight()
{
	mFacingRight = true;
	AddForce(Vector2f(250.0f * Time::DeltaTime(), 0.0f));
}

void Player::MoveLeft()
{
	AddForce(Vector2f(-250.0f * Time::DeltaTime(), 0.0f));
	mFacingRight = false;
}

void Player::Update(double deltaTime)
{
	Entity::UpdatePhysics(deltaTime);

	if (mCollider)
		mCollider->Update(deltaTime);

	if (mFacingRight)
		mAnimation = mRunningRight;
	else
		mAnimation = mRunningLeft;

	if (mAnimation)
		mAnimation->Update(deltaTime);
}

void Player::Render()
{
	if (mAnimation)
		mAnimation->Render(mRenderer, GetPosition(), GetRotation());

	if(mCollider)
		mCollider->Render(mRenderer);
}