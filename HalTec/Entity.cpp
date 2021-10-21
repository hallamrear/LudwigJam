#include "pch.h"
#include "Entity.h"
#include "Game.h"
#include "Texture.h"
#include "TextureCache.h"

Entity::Entity(std::string texture_path, Vector2f position, float rotation, float weight, float dragCoeff, float speedCap)
	: mRenderer(*Game::Renderer)
{
	mDragEnabled = false;
	mPhysicsEnabled = false;
	mGravityEnabled = false;
	mIsAlive = true;

	weight != 0.0f ? mMass = weight : mMass = 1.0f;
	speedCap != 0.0f ? mSpeedCap = speedCap : mSpeedCap = FLT_MAX;
	dragCoeff != 0.0f ? mDragCoefficient = dragCoeff : mDragCoefficient = 0.5;
	mPosition = position;
	mRotation = rotation;

	mVelocity = Vector2f();
	mAcceleration = Vector2f();
	mNetForce = Vector2f();
	mExternalForce = Vector2f();

	AssignTexture(texture_path);
}

Entity::~Entity()
{
	mTexture = nullptr;
}

void Entity::AddForce(Vector2f force)
{
	mExternalForce += force;
}

void Entity::AddForce(float X, float Y)
{
	mExternalForce.X += X;
	mExternalForce.Y += Y;
}

void Entity::AssignTexture(const std::string& texture_path)
{
	mTexture = TextureCache::GetTexture(texture_path);
}

const SDL_Renderer& Entity::GetRendererReference()
{
	return mRenderer;
}

void Entity::ClampRotation()
{
	mRotation = fmod(mRotation, 360.0f);
}

void Entity::UpdatePhysics(double deltaTime)
{
	if (mPhysicsEnabled)
	{
		if (mDragEnabled)
		{
			///Drag
			Vector2f dragForce;
			dragForce.X = -mDragCoefficient * mVelocity.X;
			dragForce.Y = -mDragCoefficient * mVelocity.Y;
			mNetForce += dragForce;
		}

		///External
		//No need for gravity
		if(mGravityEnabled)
			mNetForce += (Settings::Get()->GetGravityDirection() * mMass);

		mNetForce += mExternalForce;

		///Acceleration
		mAcceleration = Vector2f(mNetForce.X / mMass, mNetForce.Y / mMass);

		///Update Position
		mVelocity += mAcceleration * static_cast<float>(deltaTime);
		mPosition += mVelocity;

		///Speed Cap
		//Capping at 15u/s - X
		if (mVelocity.X > mSpeedCap)
			mVelocity.X = mSpeedCap;
		else if (mVelocity.X < -mSpeedCap)
			mVelocity.X = -mSpeedCap;
		//Capping at 15u/s - Y
		if (mVelocity.Y > mSpeedCap)
			mVelocity.Y = mSpeedCap;
		else if (mVelocity.Y < -mSpeedCap)
			mVelocity.Y = -mSpeedCap;

		mNetForce = Vector2f();
		mExternalForce = Vector2f();
	}
}
