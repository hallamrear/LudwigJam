#include "WorldObject.h"
#include "OrientedBoundingBox.h"
#include "Texture.h"

WorldObject::WorldObject(std::string texture, Vector2f position, float rotation)
	: Entity(texture, position, rotation, 0.0f, 0.0f, 0.0f)
{
	mCollider = new BoundingBox(mPosition, mTexture->Width, mTexture->Height);
}

WorldObject::~WorldObject()
{

}

void WorldObject::Update(double deltaTime)
{
	if (mCollider)
		mCollider->Update(deltaTime);
}

void WorldObject::Render()
{
	if (mTexture)
		mTexture->Render(mRenderer, mPosition, mRotation);

	if (mCollider)
		mCollider->Render(mRenderer);
}
