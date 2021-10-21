#include "pch.h"
#include "Animation.h"
#include "Game.h"

Animation::Animation(std::string sheetPath, unsigned int frameCount, float duration, bool looping)
	: Texture(sheetPath)
{
	mCurrentFrame = 0;
	mTimeElapsed = 0.0f;
	mLooping = looping;
	mTotalDuration = duration;
	mTotalFrames = frameCount;
	mTimeBetweenFrames = mTotalDuration / (float)(mTotalFrames);
	FrameWidth = Width / (float)mTotalFrames;
	FrameHeight = Height;
}

Animation::~Animation()
{

}

void Animation::Update(double deltaTime)
{
	if (mTexture)
	{
		mTimeElapsed += deltaTime;

		if (mTimeElapsed > mTotalDuration)
		{
			if (mLooping)
			{
				mTimeElapsed = 0.0f;
				mCurrentFrame = 0;
			}
		}
		else
			mCurrentFrame = (unsigned int)(mTimeElapsed / mTimeBetweenFrames);
	}
}

void Animation::Render(SDL_Renderer& renderer, Vector2f position, float rotation)
{
	if (mTexture)
	{
		Vector2f srcDim = Vector2f(FrameWidth, FrameHeight);
		Vector2f srcPos = Vector2f((srcDim.X * mCurrentFrame) + (srcDim.X / 2.0f), FrameHeight / 2.0f);
		Texture::Render(*Game::Renderer, position, rotation, srcPos, srcDim);
	}
}
