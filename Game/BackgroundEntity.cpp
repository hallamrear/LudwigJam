#include "BackgroundEntity.h"
#include "Animation.h"

#include "OrientedBoundingBox.h"

BackgroundEntity::BackgroundEntity(std::string texture, Vector2f position, float rotation)
	: Character("", position, rotation)
{
	mAnimation = new Animation(texture, 6, 1, true);
	mCollider = new OrientedBoundingBox(mPosition, mRotation, mAnimation->FrameWidth, mAnimation->FrameHeight);
}

BackgroundEntity::~BackgroundEntity()
{
	if(mAnimation)
	{
		delete mAnimation;
		mAnimation = nullptr;
	}
}

void BackgroundEntity::Update(double deltaTime)
{
	if (mAnimation)
		mAnimation->Update(deltaTime);

	if (mCollider)
		mCollider->Update(deltaTime);
}

void BackgroundEntity::Render()
{
	if (mAnimation)
		mAnimation->Render(mRenderer, mPosition, mRotation);

	if (mCollider)
		mCollider->Render(mRenderer);
}
