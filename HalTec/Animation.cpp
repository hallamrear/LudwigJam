#include "pch.h"
#include "Animation.h"
#include "Game.h"
#include "TextureCache.h"

AnimationController::AnimationController(std::string sheetPath, unsigned int animationCount, unsigned int frameCountPerAnimation, float duration, bool looping)
{
	mAnimationSheet = TextureCache::GetTexture(sheetPath);
	mTimeElapsed = 0.0f;
	mIsLooping = looping;
	mDuration = duration;
	mTotalFrames = frameCountPerAnimation;
	mTimeBetweenFrames = mDuration / (float)(mTotalFrames);
	FrameSize = Vector2f(mAnimationSheet->Width / (float)mTotalFrames, mAnimationSheet->Height / (float)animationCount);
}

AnimationController::~AnimationController()
{
	mAnimationSheet = nullptr;
}

void AnimationController::SetAnimation(unsigned int animation)
{
	mCurrentAnimation = animation;
}

void AnimationController::Update(double deltaTime)
{
	if (mAnimationSheet)
	{
		mTimeElapsed += deltaTime;

		if (mTimeElapsed > mDuration)
		{
			if (mIsLooping)
			{
				mTimeElapsed = 0.0f;
				mCurrentFrame = 0;
			}
		}
		else
		{
			mCurrentFrame = (unsigned int)(mTimeElapsed / mTimeBetweenFrames);
		}
	}
}

void AnimationController::Render(SDL_Renderer& renderer, Vector2f position, float rotation)
{
	if (mAnimationSheet)
	{
		Vector2f srcPos = Vector2f((FrameSize.X * mCurrentFrame) + (FrameSize.X / 2.0f), (FrameSize.Y * mCurrentAnimation) + FrameSize.Y / 2.0f);
		mAnimationSheet->Render(*Game::Renderer, position, rotation, srcPos, FrameSize);	
	}
}
