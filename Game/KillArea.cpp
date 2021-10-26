#include "KillArea.h"
#include "Player.h"
#include "OrientedBoundingBox.h"
#include "Animation.h"

KillArea::KillArea(Transform transform, Vector2f size)
	: Rigidbody("", transform, PhysicsProperties(0.0f, 0.0f, 0.0f, 0.0f, false, false, true))
{
	mAnimation = new AnimationController("Textures/Killzone.bmp", 1, 8, 2.0f, true);
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mAnimation->FrameSize.X, mAnimation->FrameSize.Y);
}

KillArea::~KillArea()
{
}

void KillArea::OnOverlap(const CollisionManifold& manifold, Rigidbody& rb)
{
	Player* player = dynamic_cast<Player*>(&rb);
	if (player)
	{
		player->SetAlive(false);
	}
}

void KillArea::Update(double deltaTime)
{
	if (mCollider)
		mCollider->Update(deltaTime);

	if (mAnimation)
		mAnimation->Update(deltaTime);
}

void KillArea::Render()
{
	if (mCollider)
		mCollider->Render(mRenderer);

	if (mAnimation)
		mAnimation->Render(mRenderer, mTransform.Position, mTransform.Rotation);
}
