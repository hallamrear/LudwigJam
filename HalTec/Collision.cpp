#include "pch.h"
#include "Collision.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"

namespace
{
	//todo : construct manifold
	bool CheckCollision_AABBvsAABB(const BoundingBox& one, const BoundingBox& two, CollisionManifold* const manifold)
	{
		//Old
		//bool collision = ((one.TopLeft.X <= two.BottomRight.X && one.BottomRight.X >= two.TopLeft.X) && (one.TopLeft.Y <= two.BottomRight.Y && one.BottomRight.Y >= two.TopLeft.Y));
		 
		//we know boxes have 4 points :)
		Vector2f pointsOne[4];
		one.GetBoxAsPoints(pointsOne);
		Vector2f pointsTwo[4];
		two.GetBoxAsPoints(pointsTwo);

		return Collision_Detection::SeperatingAxisTheory(4, pointsOne, 4, pointsTwo, manifold);
	}
	
	//todo : construct manifold
	bool CheckCollision_OBBvsSPHERE(const OrientedBoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold)
	{
		Vector2f corners[4];
		one.GetBoxAsPoints(corners);

		//transform sphere center into obb spaceand perform aabb test
		Vector2f sphereCentreAABBSpace = two.mOrigin - one.mOrigin;
		corners[0] -= one.mOrigin;
		corners[1] -= one.mOrigin;
		corners[2] -= one.mOrigin;
		corners[3] -= one.mOrigin;
		//Rotate all around 0,0 by -one.mRotaiton;
		sphereCentreAABBSpace = RotatePointAroundOriginDegrees(sphereCentreAABBSpace, 360.0f - one.Rotation, Vector2f());
		corners[0] = RotatePointAroundOriginDegrees(corners[0], 360.0f - one.Rotation, Vector2f());
		corners[1] = RotatePointAroundOriginDegrees(corners[1], 360.0f - one.Rotation, Vector2f());
		corners[2] = RotatePointAroundOriginDegrees(corners[2], 360.0f - one.Rotation, Vector2f());
		corners[3] = RotatePointAroundOriginDegrees(corners[3], 360.0f - one.Rotation, Vector2f());

		float dist = 0, minimum = INFINITY, maximum = -INFINITY;
		Vector2f extentsMin, extentsMax;
		for (int i = 0; i < 3; i++)
		{
			if (corners[i].X < extentsMin.X)
				extentsMin.X = corners[i].X;
			if (corners[i].Y < extentsMin.Y)
				extentsMin.Y = corners[i].Y;

			if (corners[i].X > extentsMax.X)
				extentsMax.X = corners[i].X;
			if (corners[i].Y > extentsMax.Y)
				extentsMax.Y = corners[i].Y;
		}

		//we square it to avoid using square roots for
		//each calculation and can use double radius at the end
		//MIN---------.
		//-			  -
		//-		      -
		//.----------MAX

		if (sphereCentreAABBSpace.X < extentsMin.X)
			dist += pow(extentsMin.X - sphereCentreAABBSpace.X, 2.0f);
		else if (sphereCentreAABBSpace.X > extentsMax.X)
			dist += pow(sphereCentreAABBSpace.X - extentsMax.X, 2.0f);

		if (sphereCentreAABBSpace.Y < extentsMin.Y)
			dist += pow(extentsMin.Y - sphereCentreAABBSpace.Y, 2.0f);
		else if (sphereCentreAABBSpace.Y > extentsMax.Y)
			dist += pow(sphereCentreAABBSpace.Y - extentsMax.Y, 2.0f);

		return dist <= two.Radius * two.Radius;
	}
	
	//todo : construct manifold
	bool CheckCollision_AABBvsSPHERE(const BoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold)
	{
		//todo : this can be improved by using obb -> aabb and running it here rather than vice versa
		float rotation = 0.0f;
		OrientedBoundingBox obb{ one.mOrigin, rotation, one.Size.X, one.Size.Y };
		return CheckCollision_OBBvsSPHERE(obb, two, manifold);
	}
	
	//todo : construct manifold
	bool CheckCollision_SPHEREvsSPHERE(const BoundingSphere& one, const BoundingSphere& two, CollisionManifold* const manifold)
	{
		Vector2f p1 = one.mOrigin;
		Vector2f p2 = two.mOrigin;
		Vector2f distance = p2 - p1;
		Vector2f distanceN = distance.GetNormalized();

		// Calculate the sum of the radii, then square it
		double sumRadii = one.Radius + two.Radius;

		if (distance.GetMagnitudeSquared() <= (sumRadii * sumRadii))
		{
			// A and B are touching
			manifold->HasCollided = true;
			manifold->Normal = distanceN;
			manifold->Depth = fabsf(distance.GetMagnitude() - sumRadii) * 0.5f;

			//todo : contact points for sphere/sphere
			float dtp = one.Radius - manifold->Depth;
			Vector2f contact = one.mOrigin + distanceN * dtp;
			manifold->ContactPositions.push_back(contact);
		}

		return manifold->HasCollided;
	};
	
	//todo : construct manifold
	bool CheckCollision_OBBvsOBB (const OrientedBoundingBox& one, const OrientedBoundingBox& two, CollisionManifold* const manifold)
	{
		//we know boxes have 4 points :)
		Vector2f pointsOne[4];
		one.GetBoxAsPoints(pointsOne);
		Vector2f pointsTwo[4];
		two.GetBoxAsPoints(pointsTwo);
		return Collision_Detection::SeperatingAxisTheory(4, pointsOne, 4, pointsTwo, manifold);
	}

	bool CheckCollision_AABBvsOBB(const BoundingBox& one, const OrientedBoundingBox& two, CollisionManifold* const manifold)
	{
		float rot = 0.0f;
		OrientedBoundingBox obb{one.mOrigin, rot, one.Size.X, one.Size.Y };
		return CheckCollision_OBBvsOBB(obb, two, manifold);
	}
}

bool Collision_Detection::CheckCollision(const Collider& one, const Collider& two, CollisionManifold* const manifold)
{
	manifold->HasCollided = false;
	manifold->Depth = 0.0f;
	manifold->Normal = Vector2f();
	manifold->ContactPositions.clear();

	if (one.mType == COLLIDER_TYPE::COLLIDER_AABB && two.mType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsAABB(dynamic_cast<const BoundingBox&>(one), dynamic_cast<const BoundingBox&>(two), manifold);

	if (one.mType == COLLIDER_TYPE::COLLIDER_SPHERE && two.mType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_SPHEREvsSPHERE(dynamic_cast<const BoundingSphere&>(one), dynamic_cast<const BoundingSphere&>(two), manifold);

	if (one.mType == COLLIDER_TYPE::COLLIDER_AABB && two.mType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_AABBvsSPHERE(dynamic_cast<const BoundingBox&>(one), dynamic_cast<const BoundingSphere&>(two), manifold);

	if (one.mType == COLLIDER_TYPE::COLLIDER_SPHERE && two.mType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsSPHERE(dynamic_cast<const BoundingBox&>(two), dynamic_cast<const BoundingSphere&>(one), manifold);

	if (one.mType == COLLIDER_TYPE::COLLIDER_OBB && two.mType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_OBBvsSPHERE(dynamic_cast<const OrientedBoundingBox&>(one), dynamic_cast<const BoundingSphere&>(two), manifold);

	if (one.mType == COLLIDER_TYPE::COLLIDER_SPHERE && two.mType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_OBBvsSPHERE(dynamic_cast<const OrientedBoundingBox&>(two), dynamic_cast<const BoundingSphere&>(one), manifold);

	if (one.mType == COLLIDER_TYPE::COLLIDER_AABB && two.mType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_AABBvsOBB(dynamic_cast<const BoundingBox&>(one), dynamic_cast<const OrientedBoundingBox&>(two), manifold);
	if (one.mType == COLLIDER_TYPE::COLLIDER_OBB && two.mType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsOBB(dynamic_cast<const BoundingBox&>(two), dynamic_cast<const OrientedBoundingBox&>(one), manifold);

	if (one.mType == COLLIDER_TYPE::COLLIDER_OBB && two.mType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_OBBvsOBB(dynamic_cast<const OrientedBoundingBox&>(one), dynamic_cast<const OrientedBoundingBox&>(two), manifold);

	throw;
	return false;
};