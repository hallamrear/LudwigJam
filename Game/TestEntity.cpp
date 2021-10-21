#include "TestEntity.h"
#include "Game.h"

#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"
#include "Texture.h"

TestEntity::TestEntity(int testmode, std::string texture, Vector2f position, float rotation)
	: Entity(texture, position, rotation, 0.0f, 0.0f, 0.0f)
{
	switch (testmode)
	{
	case 0:
		mCollider = new OrientedBoundingBox(mPosition, mRotation, mTexture->Width, mTexture->Height);
		break;
	case 1:
		mCollider = new BoundingBox(mPosition, mTexture->Width, mTexture->Height);
		break;

	case 2:
		mCollider = new BoundingSphere(mPosition, (mTexture->Width / 2.0f));
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
	if (mCollider)
		mCollider->Update(deltaTime);
}

void TestEntity::Render()
{
	if (mTexture)
		mTexture->Render(mRenderer, mPosition, mRotation);

	if (mCollider)
		mCollider->Render(mRenderer);
}
