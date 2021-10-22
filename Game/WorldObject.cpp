#include "WorldObject.h"
#include "OrientedBoundingBox.h"
#include "Texture.h"

StaticWorldObject::StaticWorldObject(std::string texture, Transform transform)
	: Entity(texture, transform, true, 0.0f, 0.0f, 0.0f, 0.0f)
{
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mTexture->Width, mTexture->Height);
}

StaticWorldObject::~StaticWorldObject()
{

}

void StaticWorldObject::Update(double deltaTime)
{
	Entity::ClampRotation();
	Entity::UpdatePhysics(deltaTime);

	if (mCollider)
		mCollider->Update(deltaTime);
}

void StaticWorldObject::Render()
{
	if (mTexture)
		mTexture->Render(mRenderer, mTransform.Position, mTransform.Rotation);

	if (mCollider)
		mCollider->Render(mRenderer);
}

DynamicWorldObject::DynamicWorldObject(std::string texture, Transform transform)
	: Entity(texture, transform, false, 0.0f, 0.0f, 0.0f, 0.0f)
{
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mTexture->Width, mTexture->Height);
}

DynamicWorldObject::~DynamicWorldObject()
{

}

void DynamicWorldObject::Update(double deltaTime)
{
	Entity::ClampRotation();
	Entity::UpdatePhysics(deltaTime);

	if (mCollider)
		mCollider->Update(deltaTime);
}

void DynamicWorldObject::Render()
{
	if (mTexture)
		mTexture->Render(mRenderer, mTransform.Position, mTransform.Rotation);

	if (mCollider)
		mCollider->Render(mRenderer);
}
