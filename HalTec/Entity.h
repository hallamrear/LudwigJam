#pragma once
#include <string>
#include "Transform.h"
class Collider;
class Texture;

struct SDL_Renderer;

class Entity
{
private:
	void					CalculateInverseMass();

protected:
	SDL_Renderer&			mRenderer;
	Texture*				mTexture;
	Collider*				mCollider;
	Transform				mTransform;
	bool					mIsAlive;
	bool					mIsStatic;

	bool					mGravityEnabled;
	bool					mDragEnabled;
	float					mMass;				//Weight of Entity (kg)
	float					mInverseMass;
	float					mRestitution;
	float					mDragCoefficient;	//Drag coefficient (default to sphere's value of 0.47)
	float					mSpeedCap;			//Speed cap (u/s)
	Vector2f				mVelocity;
	Vector2f				mAcceleration;
	Vector2f				mNetForce;
	Vector2f				mExternalForce;
	void					UpdatePhysics(double deltaTime);
	const SDL_Renderer&		GetRendererReference();
	void					ClampRotation();

public:
	Entity(
		std::string texture_path = "",
		Transform transform = Transform(),
		bool isStatic = false,
		float weight = 0.0f,
		float dragCoeff = 0.0f,
		float speedCap = 0.0f,
		float restitution = 0.0f);

	virtual					~Entity() = 0;

	void					AddForce(Vector2f force);
	void					AddForce(float X = 0.0f, float Y = 0.0f);
	//Sets mTexture to the texture at texture_path. Sets the width and height to the parameters.
	void					AssignTexture(const std::string& texture_path);

	virtual void			FixedUpdate(double deltaTime);
	virtual void			Update(double deltaTime) = 0;
	virtual void			Render() = 0;

	//Getters
	Vector2f   const GetVelocity()		 const { return mVelocity; }
	Vector2f   const GetAcceleration()	 const { return mAcceleration; }
	Transform& const GetTransform()		 	   { return mTransform; }
	bool	   const GetGravityEnabled() const { return mGravityEnabled; }
	bool	   const GetDragEnabled()	 const { return mDragEnabled; }
	Collider*  const GetCollider()		 const { return mCollider; }
	bool	   const GetIsAlive()		 const { return mIsAlive; }
	bool	   const GetIsStatic()		 const { return mIsStatic; };
	float	   const GetMass()			 const { return mMass; };
	float	   const GetInverseMass()	 const { return mInverseMass; };
	float	   const GetRestitution()	 const { return mRestitution; };

	//Setters
	//todo : remove
	virtual void	SetGravityEnabled(const bool state)	  { mGravityEnabled = state; }
	virtual void	SetDragEnabled(const bool state)	  { mDragEnabled = state; }
	virtual void	SetAlive(const bool state)			  { mIsAlive = state; }

	//Adjusters
	virtual void	AddVelocity(const Vector2f velocity)  { mVelocity += velocity; };
};

