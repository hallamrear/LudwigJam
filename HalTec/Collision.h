#pragma once
#include "Vector2.h"
#include "Helper.h"
#include <list>
#include <array>
#include <vector>

struct BoundingSphere;
struct BoundingBox;
struct OrientedBoundingBox;
struct SDL_Renderer;
class Entity;

struct CollisionManifold
{
	bool HasCollided = false;
	float Depth;
	Vector2f Normal;
};

enum class COLLIDER_TYPE
{
	COLLIDER_UNKNOWN = -1,
	COLLIDER_AABB = 0,
	COLLIDER_SPHERE,
	COLLIDER_OBB
};

//OBB - Red outline
//AABB - Green outline
//BS - Blue outline
class Collider
{
public:
	Vector2f& mOrigin;
	COLLIDER_TYPE mType;

	Collider(Vector2f& origin) : mOrigin(origin), mType(COLLIDER_TYPE::COLLIDER_UNKNOWN) { }

	virtual void Update(double deltaTime) = 0;
	virtual void Render(SDL_Renderer& renderer) = 0;

	virtual Vector2f FindFurthestPoint(Vector2f direction) const = 0;
	virtual void GetBoxAsPoints(Vector2f points[]) const = 0;
};
 
static class Collision
{
private:
	static bool CheckCollision_AABBvsAABB(const BoundingBox& one, const BoundingBox& two, CollisionManifold* const manifold);
	static bool CheckCollision_OBBvsSPHERE(const OrientedBoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold);
	static bool CheckCollision_AABBvsSPHERE(const BoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold);
	static bool CheckCollision_SPHEREvsSPHERE(const BoundingSphere& one, const BoundingSphere& two, CollisionManifold* const manifold);
	static bool CheckCollision_OBBvsOBB(const OrientedBoundingBox& one, const OrientedBoundingBox& two, CollisionManifold* const manifold);
	static bool CheckCollision_AABBvsOBB(const BoundingBox& one, const OrientedBoundingBox& two, CollisionManifold* const manifold);
	static bool SeperatingAxisTheory(const int shapeOnePointCount, const Collider& one, const int shapeTwoPointCount, const Collider& two, CollisionManifold* manifold);
	static bool SeperatingAxisTheory_Depreciated(const int shapeOnePointCount, const Collider& one, const int shapeTwoPointCount, const Collider& two, CollisionManifold* manifold);

public:
	static bool CheckCollision(const Collider& one, const Collider& two, CollisionManifold* manifold);
	static void ResolveCollision(Entity& one, Entity& two, CollisionManifold* const manifold);
};