#include "BackgroundEntity.h"
#include "Animation.h"
#include "BoundingBox.h"

BackgroundEntity::BackgroundEntity(std::string texture, Transform transform)
	: Entity("", transform)
{
	Name = "BackgroundEntity";
	mAnimation = new AnimationController(texture, 1, 6, 1, true);
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
	Entity::ClampRotation();

	if (mAnimation)
		mAnimation->Update(deltaTime);
}

void BackgroundEntity::Render()
{
	if (mAnimation)
		mAnimation->Render(mRenderer, mTransform);
}
