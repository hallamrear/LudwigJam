#include "WinnerCoin.h"
#include "OrientedBoundingBox.h"
#include "Texture.h"
#include "StateDirector.h"
#include "Player.h"
#include "Animation.h"

WinnerCoin::WinnerCoin(Transform transform)
	: Rigidbody("Textures/TotusLotus/coin_spin.bmp", transform, PhysicsProperties(0.0f, 0.0f, 0.0f, 0.0f, false, false, true))
{
	Name = "WinnerCoin";

	mCoinAnimation = new AnimationController(mTexture->GetLocation(), 1, 8, 1.0f, true);
	
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, (float)mCoinAnimation->FrameSize.X, (float)mCoinAnimation->FrameSize.Y);
	mCollider->IsOverlap = true;

	mIsCollected = false;
}

WinnerCoin::~WinnerCoin()
{
	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}

	if (mCoinAnimation)
	{
		delete mCoinAnimation;
		mCoinAnimation = nullptr;
	}
}

void WinnerCoin::OnOverlap(const CollisionManifold& manifold, Rigidbody& rb)
{
	mIsCollected = true;

	Player* player = dynamic_cast<Player*>(&rb);
	if (player)
	{
		StateDirector::SetState(GameStateIdentifier::GAME_STATE_1);
	}
}

void WinnerCoin::Update(double deltaTime)
{
	if (mCoinAnimation)
	{
		mCoinAnimation->Update(deltaTime);
	}

	if (mCollider)
	{
		mCollider->Update(deltaTime);
	}
}

void WinnerCoin::Render()
{
	if (mCoinAnimation)
	{
		mCoinAnimation->Render(mRenderer, mTransform);
	}

	if (mCollider)
	{
		mCollider->Render(mRenderer);
	}
}

const bool& WinnerCoin::HasBeenCollected() const
{
	return mIsCollected;
}
