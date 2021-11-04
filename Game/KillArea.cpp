#include "KillArea.h"
#include "Player.h"
#include "OrientedBoundingBox.h"
#include "TextureCache.h"
#include "Texture.h"

KillArea::KillArea(Transform transform, int size)
	: Rigidbody("", transform, PhysicsProperties(0.0f, 0.0f, 0.0f, 0.0f, false, false, true))
{
	switch (size)
	{
	case 1:
		mTexture = TextureCache::GetTexture("Textures/killbox-small.bmp");
		break;
	case 2:
		mTexture = TextureCache::GetTexture("Textures/killbox-mid.bmp");
		break;
	case 3:
		mTexture = TextureCache::GetTexture("Textures/killbox-large.bmp");
		break;

	default:
		break;
	}

	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, (float)mTexture->Width, (float)mTexture->Height);
	mCollider->IsOverlap = true;
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
}

void KillArea::Render()
{
	if (mCollider)
		mCollider->Render(mRenderer);

	if (mTexture)
		mTexture->Render(mRenderer, mTransform.Position, mTransform.Rotation);
}
