#pragma once
#include "Texture.h"

class Animation : public Texture
{
	bool mLooping;
	unsigned int mCurrentFrame;
	unsigned int mTotalFrames;
	float mTimeBetweenFrames;
	float mTotalDuration;
	float mTimeElapsed;

public:
	int FrameWidth;
	int FrameHeight;

	Animation(std::string sheetPath, unsigned int frameCount, float duration, bool looping);
	~Animation();
	
	void Update(double deltaTime);
	void Render(SDL_Renderer& renderer, Vector2f position, float rotation);
};

