#pragma once
#include "Vector2.h"
#include "Helper.h"
#include <list>
#include <array>
#include <vector>

struct SDL_Renderer;

struct CollisionManifold
{
	bool HasCollided = false;
	float Depth;
	Vector2f Normal;
	std::vector<Vector2f> ContactPositions;
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
};
 
namespace Collision_Detection
{
	namespace
	{
		bool SeperatingAxisTheory(const int shapeOnePointCount, const Vector2f shapeOnePoints[], const int shapeTwoPointCount, const Vector2f shapeTwoPoints[], CollisionManifold* manifold)
		{
			const Vector2f* pointsOne = shapeOnePoints;
			const Vector2f* pointsTwo = shapeTwoPoints;
			manifold->HasCollided = true;

			//todo : remove
			float output[4];

			//todo : Make this useable with n points.
			for (int s = 0; s < 2; s++)
			{
				if (s == 1)
				{
					pointsTwo = shapeOnePoints;
					pointsOne = shapeTwoPoints;
				}
				else
				{
					pointsOne = shapeOnePoints;
					pointsTwo = shapeTwoPoints;
				}

				//Check shape one in each direction
				for (size_t a = 0; a < shapeOnePointCount; a++)
				{
					//wraparound
					int b = (a + 1) % shapePointCount;

					Vector2f axisProj = Vector2f(-(pointsOne[b].Y - pointsOne[a].Y), (pointsOne[b].X - pointsOne[a].X));

					float min_r1 = INFINITY, max_r1 = -INFINITY, min_r2 = INFINITY, max_r2 = -INFINITY;
					for (size_t P = 0; P < shapePointCount; P++)
					{
						//project each point onto line 
						float q_one = pointsOne[P].Dot(axisProj);
						float q_two = pointsTwo[P].Dot(axisProj);

						//get the min and max of the projection extents
						min_r1 = std::min(min_r1, q_one);
						max_r1 = std::max(max_r1, q_one);
						min_r2 = std::min(min_r2, q_two);
						max_r2 = std::max(max_r2, q_two);

						output[0] = min_r1;
						output[1] = max_r1;
						output[2] = min_r2;
						output[3] = max_r2;
					}

					//if they overlap, continue else if they dont, theyre not colliding so can return
					if (!(max_r2 >= min_r1) && (max_r1 >= min_r2))
					{
						manifold->HasCollided = false;
						break;
						//return false;
					}
				}
			}

			//todo : construct manifold
			return manifold->HasCollided;
		}
	}

	bool CheckCollision(const Collider& one, const Collider& two, CollisionManifold* manifold);
}
