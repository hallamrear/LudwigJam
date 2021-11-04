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
#include "Projectile.h"

Player::Player(std::string texture, Transform transform, PhysicsProperties properties)
	: Rigidbody(texture, transform, properties)
{
	mIsStatic = false;

	

	SetupInput();

	mFacingRight = true;
	mAnimation = new AnimationController("Textures/testSpriteSheet.bmp", 6, 8, 1, true);

	//mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mAnimation->FrameSize.X, mAnimation->FrameSize.Y);
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, 20.0f, mAnimation->FrameSize.Y);
}

Player::~Player()
{
	if(mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}

	if (mAnimation)
	{
		delete mAnimation;
		mAnimation = nullptr;
	}
}

void Player::SetupInput()
{
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN, IM_KEY_STATE::IM_KEY_PRESSED, [this] { MoveRight(); });
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP, IM_KEY_STATE::IM_KEY_PRESSED, [this] { MoveLeft(); });
	//InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this] { Shoot(); });
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_MIDDLE_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this] 
		{ 
			AddForce(mTransform.GetUp() * 20000.0f);
		});
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

void Player::Shoot()
{
	Transform transform = GetTransform();

	if (mFacingRight)
		transform.AdjustPosition(mTransform.GetRight() * 50.0f);
	else
		transform.AdjustPosition(mTransform.GetLeft() * 50.0f);

	projs.push_back(new Projectile(transform));

	if (mFacingRight)
		projs.back()->AddVelocity(Vector2f(transform.GetRight() * 10));
	else
		projs.back()->AddVelocity(Vector2f(transform.GetLeft() * 10));
}

void Player::Update(double deltaTime)
{
	Log::LogMessage(LogLevel::LOG_ERROR, std::to_string(projs.size()));

	if (projs.size() > 0)
	{
		std::vector<int> toErase;
		int size = (int)projs.size();
		for (int i = 0; i < size; i++)
		{
			if (projs[i]->GetIsAlive())
				projs[i]->Update(deltaTime);
			else
				toErase.push_back(i);
		}

		size = (int)toErase.size();
		for (int i = 0; i < size; i++)
		{
			projs.erase(projs.begin() + toErase[i]);
		}

		toErase.clear();
	}

		
	if (mCollider)
		mCollider->Update(deltaTime);

	if (abs(mVelocity.X) > 0.1f)
	{
		if (mFacingRight)
			mAnimation->SetAnimation(0);
		else
			mAnimation->SetAnimation(2);
	}
	else
	{
		mAnimation->SetAnimation(1);
	}

	if (mAnimation)
		mAnimation->Update(deltaTime);
}

void Player::Render()
{	
	for (auto& itr : projs)
		itr->Render();

	if (mAnimation)
		mAnimation->Render(mRenderer, mTransform.Position, mTransform.Rotation);

	if(mCollider)
		mCollider->Render(mRenderer);
}