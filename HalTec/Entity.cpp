#include "pch.h"
#include "Entity.h"
#include "Game.h"
#include "Texture.h"
#include "TextureCache.h"

Entity::Entity(std::string texture_path, Transform transform, bool isStatic, float weight, float dragCoeff, float speedCap, float restitution)
	: mRenderer(*Game::Renderer)
{
	mTransform = transform;
	mDragEnabled = false;
	mGravityEnabled = false;
	mIsAlive = true;
	mIsStatic = isStatic;

	restitution != -1.0f ? mRestitution = restitution : mRestitution = 0.5f;
	weight != 0.0f ? mMass = weight : mMass = 1.0f;
	speedCap != 0.0f ? mSpeedCap = speedCap : mSpeedCap = 1000000.0f;
	dragCoeff != 0.0f ? mDragCoefficient = dragCoeff : mDragCoefficient = 0.5;

	mVelocity = Vector2f();
	mAcceleration = Vector2f();
	mNetForce = Vector2f();
	mExternalForce = Vector2f();

	AssignTexture(texture_path);
	CalculateInverseMass();
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

void Entity::FixedUpdate(double deltaTime)
{
	UpdatePhysics(deltaTime);
}

const SDL_Renderer& Entity::GetRendererReference()
{
	return mRenderer;
}

void Entity::ClampRotation()
{
	mTransform.Rotation = fmod(mTransform.Rotation, 360.0f);
}

void Entity::CalculateInverseMass()
{
	if (GetIsStatic())
	{
		mInverseMass = 0.0f;
	}
	else
	{
		mInverseMass = 1.0f / mMass;
	}
}

void Entity::UpdatePhysics(double deltaTime)
{
	if (mIsStatic)
		return;

	if (GetDragEnabled())
	{
		///Drag
		Vector2f dragForce;
		dragForce.X = -mDragCoefficient * mVelocity.X;
		dragForce.Y = -mDragCoefficient * mVelocity.Y;
		mNetForce += dragForce;
	}

	///External
	//No need for gravity
	if(GetGravityEnabled())
		mNetForce += (Settings::Get()->GetGravityDirection() * mMass);

	mNetForce += mExternalForce;

	///Acceleration
	mAcceleration = Vector2f(mNetForce.X / mMass, mNetForce.Y / mMass);

	///Update Position
	mVelocity += mAcceleration * static_cast<float>(deltaTime);
	mTransform.Position += mVelocity;

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
