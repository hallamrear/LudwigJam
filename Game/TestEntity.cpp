#include "TestEntity.h"
#include "Game.h"

#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"
#include "Texture.h"

TestEntity::TestEntity(int testmode, std::string texture, Transform transform)
	: Entity(texture, transform, false, 0.0f, 0.0f, 0.0f, 0.0f)
{
	mMass = rand() % 500 + 1;
	SetGravityEnabled(true);

	switch (testmode)
	{
	case 0:
		mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mTexture->Width, mTexture->Height);
		break;
	case 1:
		mCollider = new BoundingBox(mTransform.Position, mTexture->Width, mTexture->Height);
		break;
	case 2:
		mCollider = new BoundingSphere(mTransform.Position, (mTexture->Width / 2.0f));
		break;
	default:
		break;
	}

}

TestEntity::~TestEntity()
{

}

void TestEntity::Update(double deltaTime)
{
	Entity::ClampRotation();
	Entity::UpdatePhysics(deltaTime);

	if (mCollider)
		mCollider->Update(deltaTime);
}

void TestEntity::Render()
{
	if (mTexture)
		mTexture->Render(mRenderer, mTransform.Position, mTransform.Rotation);

	if (mCollider)
		mCollider->Render(mRenderer);
}
