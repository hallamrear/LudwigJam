#include "WorldObject.h"
#include "OrientedBoundingBox.h"
#include "Texture.h"

StaticWorldObject::StaticWorldObject(std::string texture, Transform transform)
	: Rigidbody(texture, transform, PhysicsProperties(100000.0f, 1.0f, 1000.0f, 0.5f, false, false, true))
{
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mTexture->Width, mTexture->Height);
}

StaticWorldObject::~StaticWorldObject()
{
	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void StaticWorldObject::Update(double deltaTime)
{
	Entity::ClampRotation();

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

DynamicWorldObject::DynamicWorldObject(std::string texture, Transform transform, PhysicsProperties properties)
	: Rigidbody(texture, transform, properties)
{
	mIsStatic = false;
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mTexture->Width, mTexture->Height);
}

DynamicWorldObject::~DynamicWorldObject()
{
	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void DynamicWorldObject::Update(double deltaTime)
{
	Entity::ClampRotation();

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
