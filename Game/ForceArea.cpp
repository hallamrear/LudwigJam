#include "ForceArea.h"
#include "OrientedBoundingBox.h"
#include "Rigidbody.h"
#include "Animation.h"
#include "Log.h"
#include "Player.h"

ForceArea::ForceArea(Transform transform, Vector2f size, int forceStrength)
	: Rigidbody("", transform, PhysicsProperties(100.0f, 0.0f, 0.0f, 0.0f, false, false, true))
{
	mForceStrength = forceStrength;
	mSize = size;

	mAnimation = new AnimationController("Textures/Wind.bmp", 1, 4, 1.0f, true);
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mSize.X, mSize.Y);
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
		rb->AddExternalForce(mTransform.GetRight() * (float)mForceStrength);
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
		mAnimation->Render(mRenderer, mTransform.Position, mTransform.Rotation);
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
