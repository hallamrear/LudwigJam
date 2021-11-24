#include "ForceArea.h"
#include "OrientedBoundingBox.h"
#include "Rigidbody.h"
#include "Animation.h"
#include "Log.h"
#include "Player.h"

ForceArea::ForceArea(Transform transform, int areaSize, int forceStrength)
	: Rigidbody("", transform, PhysicsProperties(100.0f, 0.0f, 0.0f, 0.0f, false, false, true))
{
	Name = "ForceZone";
	mForceStrength = forceStrength;

	switch (areaSize)
	{

	default:
	case 1:
		mAnimation = new AnimationController("Textures/forceAreaSmall.bmp", 1, 16, 1.0f, true);
		break;

	case 2:
		mAnimation = new AnimationController("Textures/forceAreaMid.bmp", 1, 16, 1.0f, true);
		break;

	case 3:
		mAnimation = new AnimationController("Textures/forceAreaLarge.bmp", 1, 16, 1.0f, true);
		break;
	}
	
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mAnimation->FrameSize.X, mAnimation->FrameSize.Y);
	mCollider->IsOverlap = true;
}

ForceArea::~ForceArea()
{
	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void ForceArea::ApplyForce(Rigidbody* rb)
{
	if (rb)
		rb->AddExternalForce(mTransform.GetUp() * (float)mForceStrength);
}

void ForceArea::Update(double deltaTime)
{
	if (mCollider)
		mCollider->Update(deltaTime);

	if (mAnimation)
		mAnimation->Update(deltaTime);
}

void ForceArea::Render()
{
	if (mCollider)
		mCollider->Render(mRenderer);

	if (mAnimation)
		mAnimation->Render(mRenderer, mTransform);
}

void ForceArea::OnOverlap(const CollisionManifold& manifold, Rigidbody& other)
{
	ApplyForce(&other);
}

void ForceArea::OnCollision(const CollisionManifold& manifold, Rigidbody& other)
{
	if (dynamic_cast<Player*>(&other))
		Log::LogMessage(LogLevel::LOG_MESSAGE, "ON COLLISION");
}

Collider* ForceArea::GetCollider()
{
	if (mCollider)
		return (Collider*)mCollider;
	else
		return nullptr;
}
