#include <string>

#include "Player.h"
#include "InputManager.h"
#include "Log.h"
#include "OrientedBoundingBox.h"
#include "Animation.h"
#include "Vector2.h"

Player::Player(std::string texture, Transform transform, PhysicsProperties properties)
	: Rigidbody(texture, transform, properties)
{
	mIsStatic = false;
	mIsJumping = false;
	SetupInput();

	mFacingRight = true;
	mAnimation = new AnimationController("Textures/testSpriteSheet.bmp", 6, 8, 1, true);
	mJumpingAnimation = new AnimationController("Textures/PlayerJumping.bmp", 1, 3, 0.5f, false);

	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, 20.0f, mAnimation->FrameSize.Y - 5.0f);
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
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN, IM_KEY_STATE::IM_KEY_PRESSED, [this] { MoveRight();});
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP, IM_KEY_STATE::IM_KEY_PRESSED, [this] { MoveLeft();});
	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_MIDDLE_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this] 
		{ 
			mIsJumping = true;
			mJumpingAnimation->Start();
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

void Player::Update(double deltaTime)
{		
	if (mCollider)
		mCollider->Update(deltaTime);

	if (mIsJumping)
	{
		if (mJumpingAnimation->HasFinished())
			mIsJumping = false;
	}
	else
	{
		if (abs(mVelocity.X) > 0.1f)
		{
			if (mFacingRight)
				mAnimation->SetAnimation(0);
			else
				mAnimation->SetAnimation(2);
		}
		else
			mAnimation->SetAnimation(1);
	}

	if (mAnimation)
		mAnimation->Update(deltaTime);

	if (mJumpingAnimation)
		mJumpingAnimation->Update(deltaTime);
}

void Player::Render()
{	
	if (mJumpingAnimation)
	{
		if (mJumpingAnimation->HasFinished())
		{
			mIsJumping = false;

			if (mAnimation)
				mAnimation->Render(mRenderer, mTransform);
		}
		else
		{
			if(mFacingRight)
				mJumpingAnimation->Render(mRenderer, mTransform, true);
			else
				mJumpingAnimation->Render(mRenderer, mTransform, false);
		}
	}

	if(mCollider)
		mCollider->Render(mRenderer);
}