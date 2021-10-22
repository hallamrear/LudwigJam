#pragma once
#include <math.h>

//todo: remove or replace
/*
* 
	Vector3f Cross(Vector3f other)
	{
		Vector3f result;
		result.X = (Y * other.Z) - (Z * other.Y);
		result.Y = (Z * other.X) - (X * other.Z);
		result.Z = (X * other.Y) - (Y * other.X);
		return result;
	}

	inline static Vector3f Cross(Vector3f vectorA, Vector3f vectorB)
	{
		Vector3f result;
		result.X = (vectorA.Y * vectorB.Z) - (vectorA.Z * vectorB.Y);
		result.Y = (vectorA.Z * vectorB.X) - (vectorA.X * vectorB.Z);
		result.Z = (vectorA.X * vectorB.Y) - (vectorA.Y * vectorB.X);
		return result;
	}
*/

class Vector2f
{
public:

	float X;
	float Y;

	inline Vector2f()
	{
		X = 0.0f;
		Y = 0.0f;
	}

	inline Vector2f(float x, float y)
	{
		X = x;
		Y = y;
	}

	
	inline float GetMagnitude() const
	{
		//Pythagoras
		return sqrtf((X * X) + (Y * Y));
	}

	inline float GetMagnitudeSquared() const
	{
		return (X * X) + (Y * Y);
	}

	inline Vector2f GetNormalized() const
	{
		float magnitude = GetMagnitude();
		return Vector2f(X / magnitude, Y / magnitude);
	}

	//Operators
	Vector2f& operator= (const Vector2f& other)
	{
		this->X = other.X;
		this->Y = other.Y;

		return *this;
	}

	Vector2f operator+(const Vector2f& rhs)
	{
		return Vector2f(this->X + rhs.X, this->Y + rhs.Y);
	}

	Vector2f operator-(const Vector2f& rhs)
	{
		return Vector2f(this->X - rhs.X, this->Y - rhs.Y);
	}

	Vector2f operator*(const float& scalar)
	{
		return Vector2f(this->X * scalar, this->Y * scalar);
	}

	Vector2f& operator+=(const Vector2f& rhs)
	{
		this->X += rhs.X;
		this->Y += rhs.Y;
		return *this;
	}

	Vector2f& operator-=(const Vector2f& rhs)
	{
		this->X -= rhs.X;
		this->Y -= rhs.Y;
		return *this;
	}

	bool operator!=(const Vector2f& rhs)
	{
		if ((this->X == rhs.X) && (this->Y == rhs.Y))
			return false;
		else
			return true;
	}

	float Dot(Vector2f vector) const
	{
		return ((this->X * vector.X) + (this->Y * vector.Y));
	}

	bool operator==(const Vector2f& rhs)
	{
		if ((this->X == rhs.X) && (this->Y == rhs.Y))
			return true;
		else
			return false;
	}

	friend bool operator==(const Vector2f& lhs, const Vector2f& rhs)
	{
		if ((lhs.X == rhs.X) && (lhs.Y == rhs.Y))
			return true;
		else
			return false;
	}

	friend Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs)
	{
		return Vector2f(lhs.X - rhs.X, lhs.Y - rhs.Y);
	}
};