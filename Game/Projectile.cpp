#include "Projectile.h"
#include "OrientedBoundingBox.h"
#include "Animation.h"

Projectile::Projectile(Transform transform)
	: Rigidbody("", transform, PhysicsProperties(50.0f, 1.0f, 5000.0f, 0.47f, false, false, false))
{
	mAnimation = new AnimationController("Textures/Projectile.bmp", 1, 2, 0.25f, true);
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, 10.0f, 10.0f);
	mCollider->IsOverlap = true;
}

Projectile::~Projectile()
{
	if (mAnimation)
	{
		delete mAnimation;
		mAnimation = nullptr;
	}

	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void Projectile::Update(double deltaTime)
{
	if (mIsAlive)
	{
		if (mCollider)
			mCollider->Update(deltaTime);

		if (mAnimation)
			mAnimation->Update(deltaTime);
	}
}

void Projectile::Render()
{
	if (mIsAlive)
	{
		//Todo : mTexture Render take in transform instead
		if (mAnimation)
			mAnimation->Render(mRenderer, mTransform.Position, mTransform.Rotation);

		if (mCollider)
			mCollider->Render(mRenderer);
	}
}

void Projectile::OnOverlap(const CollisionManifold& manifold, Rigidbody& other)
{
	if(!dynamic_cast<Projectile*>(&other))
		mIsAlive = false;
}