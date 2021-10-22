#include "pch.h"
#include "Collision.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"
#include "Entity.h"

//todo : construct manifold
bool Collision::CheckCollision_AABBvsAABB(const BoundingBox& one, const BoundingBox& two, CollisionManifold* const manifold)
{
	//Old
	//bool collision = ((one.TopLeft.X <= two.BottomRight.X && one.BottomRight.X >= two.TopLeft.X) && (one.TopLeft.Y <= two.BottomRight.Y && one.BottomRight.Y >= two.TopLeft.Y));

	//we know boxes have 4 points :)
	return Collision::SeperatingAxisTheory(4, one, 4, two, manifold);
}

//todo : construct manifold
bool Collision::CheckCollision_OBBvsSPHERE(const OrientedBoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold)
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
bool Collision::CheckCollision_AABBvsSPHERE(const BoundingBox& one, const BoundingSphere& two, CollisionManifold* const manifold)
{
	//todo : this can be improved by using obb -> aabb and running it here rather than vice versa
	float rotation = 0.0f;
	OrientedBoundingBox obb{ one.mOrigin, rotation, one.Size.X, one.Size.Y };
	return CheckCollision_OBBvsSPHERE(obb, two, manifold);
}

//todo : construct manifold
bool Collision::CheckCollision_SPHEREvsSPHERE(const BoundingSphere& one, const BoundingSphere& two, CollisionManifold* const manifold)
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
	}

	return manifold->HasCollided;
};

//todo : construct manifold
bool Collision::CheckCollision_OBBvsOBB(const OrientedBoundingBox& one, const OrientedBoundingBox& two, CollisionManifold* const manifold)
{
	//we know boxes have 4 points :)
	return Collision::SeperatingAxisTheory(4, one, 4, two, manifold);
}

bool Collision::CheckCollision_AABBvsOBB(const BoundingBox& one, const OrientedBoundingBox& two, CollisionManifold* const manifold)
{
	return SeperatingAxisTheory(4, one, 4, two, manifold);
}

bool Collision::SeperatingAxisTheory(const int shapeOnePointCount, const Collider& one, const int shapeTwoPointCount, const Collider& two, CollisionManifold* manifold)
{
	Vector2f* shapeOnePoints = new Vector2f[shapeOnePointCount];
	one.GetBoxAsPoints(shapeOnePoints);
	Vector2f* shapeTwoPoints = new Vector2f[shapeTwoPointCount];
	two.GetBoxAsPoints(shapeTwoPoints);

	manifold->Normal = Vector2f(0.0f, 0.0f);
	manifold->Depth = FLT_MAX;

	//Check shape one in each direction
	for (size_t a = 0; a < shapeOnePointCount; a++)
	{
		//wraparound
		int b = (a + 1) % shapeOnePointCount;

		Vector2f axisProj = Vector2f(-(shapeOnePoints[b].Y - shapeOnePoints[a].Y), (shapeOnePoints[b].X - shapeOnePoints[a].X));
		axisProj = axisProj.GetNormalized();

		float min_r1 = INFINITY, max_r1 = -INFINITY, min_r2 = INFINITY, max_r2 = -INFINITY;
		for (size_t P = 0; P < shapeOnePointCount; P++)
		{
			//project each point onto line 
			float q_one = shapeOnePoints[P].Dot(axisProj);

			//get the min and max of the projection extents
			min_r1 = std::min(min_r1, q_one);
			max_r1 = std::max(max_r1, q_one);
		}

		for (size_t P = 0; P < shapeTwoPointCount; P++)
		{
			//project each point onto line 
			float q_two = shapeTwoPoints[P].Dot(axisProj);

			//get the min and max of the projection extents
			min_r2 = std::min(min_r2, q_two);
			max_r2 = std::max(max_r2, q_two);
		}

		//if they overlap, continue else if they dont, theyre not colliding so can return
		if (!(max_r2 >= min_r1) && (max_r1 >= min_r2))
		{
			delete[] shapeOnePoints;
			shapeOnePoints = nullptr;
			delete[] shapeTwoPoints;
			shapeTwoPoints = nullptr;

			manifold->HasCollided = false;
			return manifold->HasCollided;
		}

		float axisDepth = std::min(max_r2 - min_r1, max_r1 - min_r2);

		if (axisDepth < manifold->Depth)
		{
			manifold->Depth = axisDepth;
			manifold->Normal = axisProj;
		}
	}

	for (size_t a = 0; a < shapeTwoPointCount; a++)
	{
		//wraparound
		int b = (a + 1) % shapeTwoPointCount;

		Vector2f axisProj = Vector2f(-(shapeTwoPoints[b].Y - shapeTwoPoints[a].Y), (shapeTwoPoints[b].X - shapeTwoPoints[a].X));
		axisProj = axisProj.GetNormalized();

		float min_r1 = INFINITY, max_r1 = -INFINITY, min_r2 = INFINITY, max_r2 = -INFINITY;
		for (size_t P = 0; P < shapeOnePointCount; P++)
		{
			//project each point onto line 
			float q_one = shapeOnePoints[P].Dot(axisProj);

			//get the min and max of the projection extents
			min_r1 = std::min(min_r1, q_one);
			max_r1 = std::max(max_r1, q_one);
		}

		for (size_t P = 0; P < shapeTwoPointCount; P++)
		{
			//project each point onto line 
			float q_two = shapeTwoPoints[P].Dot(axisProj);

			//get the min and max of the projection extents
			min_r2 = std::min(min_r2, q_two);
			max_r2 = std::max(max_r2, q_two);
		}

		//if they overlap, continue else if they dont, theyre not colliding so can return
		if (!(max_r2 >= min_r1) && (max_r1 >= min_r2))
		{
			delete[] shapeOnePoints;
			shapeOnePoints = nullptr;
			delete[] shapeTwoPoints;
			shapeTwoPoints = nullptr;

			manifold->HasCollided = false;
			return manifold->HasCollided;
		}

		float axisDepth = std::min(max_r2 - min_r1, max_r1 - min_r2);

		if (axisDepth < manifold->Depth)
		{
			manifold->Depth = axisDepth;
			manifold->Normal = axisProj;
		}
	}


	//todo : if you uncomment this, remove the axisproj.getnormalized
	//manifold->Depth /= manifold->Normal.GetMagnitude();
	//manifold->Normal = manifold->Normal.GetNormalized();

	Vector2f direction = two.mOrigin - one.mOrigin;

	if (direction.Dot(manifold->Normal) < 0.0f)
	{
		manifold->Normal = manifold->Normal * -1;
	}

	delete[] shapeOnePoints;
	shapeOnePoints = nullptr;
	delete[] shapeTwoPoints;
	shapeTwoPoints = nullptr;

	manifold->HasCollided = true;
	return manifold->HasCollided;
}

bool Collision::SeperatingAxisTheory_Depreciated(const int shapeOnePointCount, const Collider& one, const int shapeTwoPointCount, const Collider& two, CollisionManifold* manifold)
{
	Vector2f* shapeOnePoints = new Vector2f[shapeOnePointCount];
	one.GetBoxAsPoints(shapeOnePoints);
	Vector2f* shapeTwoPoints = new Vector2f[shapeTwoPointCount];
	two.GetBoxAsPoints(shapeTwoPoints);

	manifold->Normal = Vector2f(0.0f, 0.0f);
	manifold->Depth = FLT_MAX;

	//Check shape one in each direction
	for (size_t a = 0; a < shapeOnePointCount; a++)
	{
		//wraparound
		int b = (a + 1) % shapeOnePointCount;
		
		Vector2f edge = shapeTwoPoints[b] - shapeTwoPoints[a];

		//Get the axis for projection at 90 to edge
		Vector2f axisProj = Vector2f(-edge.Y, edge.X);
		axisProj = axisProj.GetNormalized();

		Vector2f vertex;
		float minS1 = INFINITY, maxS1 = -INFINITY, minS2 = INFINITY, maxS2 = -INFINITY;
		for (size_t i = 0; i < shapeOnePointCount; i++)
		{
			vertex = shapeOnePoints[i];
			float projected = vertex.Dot(axisProj);

			minS1 = std::min(minS1, projected);
			maxS1 = std::max(maxS1, projected);
		}

		for (size_t i = 0; i < shapeTwoPointCount; i++)
		{
			vertex = shapeTwoPoints[i];
			float projected = vertex.Dot(axisProj);

			minS2 = std::min(minS2, projected);
			maxS2 = std::max(maxS2, projected);
		}

		//if they overlap, continue else if they dont, theyre not colliding so can return
		//if (minS1 >= maxS2 || minS2 >= maxS1)
		if (!(maxS2 >= minS1) && (maxS1 >= minS2))
		{
			delete[] shapeOnePoints;
			shapeOnePoints = nullptr;
			delete[] shapeTwoPoints;
			shapeTwoPoints = nullptr;

			manifold->HasCollided = false;
			return manifold->HasCollided;
		}

		float axisDepth = std::min(maxS2 - minS1, maxS1 - minS2);

		if (axisDepth < manifold->Depth)
		{
			manifold->Depth = axisDepth;
			manifold->Normal = axisProj;
		}
	}

	//Check shape two in each direction
	for (size_t a = 0; a < shapeTwoPointCount; a++)
	{
		//wraparound
		int b = (a + 1) % shapeTwoPointCount;

		Vector2f edge = shapeTwoPoints[b] - shapeTwoPoints[a];

		//Get the axis for projection at 90 to edge
		Vector2f axisProj = Vector2f(-edge.Y, edge.X);
		axisProj = axisProj.GetNormalized();

		Vector2f vertex;
		float minS1 = INFINITY, maxS1 = -INFINITY, minS2 = INFINITY, maxS2 = -INFINITY;
		for (size_t i = 0; i < shapeOnePointCount; i++)
		{
			vertex = shapeOnePoints[i];
			float projected = vertex.Dot(axisProj);

			minS1 = std::min(minS1, projected);
			maxS1 = std::max(maxS1, projected);
		}

		for (size_t i = 0; i < shapeTwoPointCount; i++)
		{
			vertex = shapeTwoPoints[i];
			float projected = vertex.Dot(axisProj);

			minS2 = std::min(minS2, projected);
			maxS2 = std::max(maxS2, projected);
		}

		//if they overlap, continue else if they dont, theyre not colliding so can return
		//if (minS1 >= maxS2 || minS2 >= maxS1) 
		if (!(maxS2 >= minS1) && (maxS1 >= minS2))
		{
			delete[] shapeOnePoints;
			shapeOnePoints = nullptr;
			delete[] shapeTwoPoints;
			shapeTwoPoints = nullptr;

			manifold->HasCollided = false;
			return manifold->HasCollided;
		}

		float axisDepth = std::min(maxS2 - minS1, maxS1 - minS2);

		if (axisDepth < manifold->Depth)
		{
			manifold->Depth = axisDepth;
			manifold->Normal = axisProj;
		}
	}

	//todo: Add method to get centre of all vertices rather than origins
	//e.g. center = FindPointCloudCenterMean(verticesA);

	manifold->Depth /= manifold->Normal.GetMagnitude();
	manifold->Normal = manifold->Normal.GetNormalized();

	Vector2f direction = two.mOrigin - one.mOrigin;

	if (direction.Dot(manifold->Normal) < 0.0f)
	{
		manifold->Normal = manifold->Normal * -1;
	}

	delete[] shapeOnePoints;
	shapeOnePoints = nullptr;
	delete[] shapeTwoPoints;
	shapeTwoPoints = nullptr;

	//todo : construct manifold
	manifold->HasCollided = true;
	return manifold->HasCollided;
}

bool Collision::CheckCollision(const Collider& one, const Collider& two, CollisionManifold* const manifold)
{
	manifold->HasCollided = false;
	manifold->Depth = 0.0f;
	manifold->Normal = Vector2f();

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
}

void Collision::ResolveCollision(Entity& one, Entity& two, CollisionManifold* const manifold)
{
	if (!manifold)
		return;

	//Do nothing, do not resolve
	//Static - Static
	if (one.GetIsStatic() == true && two.GetIsStatic() == true)
		return;			  

	Vector2f relativeVelocity = two.GetVelocity() - one.GetVelocity();

	//objects are moving apart
	if (relativeVelocity.Dot(manifold->Normal) > 0.0f)
		return;

	//Equations are based on
	//https://www.chrishecker.com/Rigid_Body_Dynamics

	////Static - Dynamic
	if (one.GetIsStatic() == true && two.GetIsStatic() == false)
	{
		two.GetTransform().AdjustPosition(manifold->Normal * (manifold->Depth));
	}
	//Dynamic - Static
	else if (one.GetIsStatic() == false && two.GetIsStatic() == true)
	{
		one.GetTransform().AdjustPosition(manifold->Normal * (manifold->Depth));
	}
	//Dynamic - Dynamic
	//Move both away from each other.
	else
	{
		one.GetTransform().AdjustPosition(manifold->Normal * -1 * (manifold->Depth / 2.0f));
		two.GetTransform().AdjustPosition(manifold->Normal * (manifold->Depth / 2.0f));
	}

	//Get minimum restitution;
	float e = std::min(one.GetRestitution(), two.GetRestitution());
	float mag = -(1.0f + e) * relativeVelocity.Dot(manifold->Normal);
	mag /= one.GetInverseMass() + two.GetInverseMass();

	Vector2f impulse = manifold->Normal * mag;
	one.AddVelocity(impulse * -one.GetInverseMass());
	two.AddVelocity(impulse * two.GetInverseMass());	
}