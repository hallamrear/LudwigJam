#include "pch.h"
#include "BoundingBox.h"
#include "Camera.h"

BoundingBox::BoundingBox(Vector2f& position, float size_x, float size_y)
	: Collider(position)
{
	mType = COLLIDER_TYPE::COLLIDER_AABB;

	Size.X = size_x;
	Size.Y = size_y;

	TopLeft = Vector2f(position.X - (Size.X / 2.0f), position.Y + (Size.Y / 2.0f));
	BottomRight = Vector2f(position.X + (Size.X / 2.0f), position.Y - (Size.Y / 2.0f));
};

BoundingBox::~BoundingBox()
{
	
};

void BoundingBox::Update(double deltaTime)
{
	TopLeft = Vector2f(mOrigin.X - (Size.X / 2), mOrigin.Y + (Size.Y / 2));
	BottomRight = Vector2f(mOrigin.X + (Size.X / 2), mOrigin.Y - (Size.Y / 2));
}

void BoundingBox::Render(SDL_Renderer& renderer)
{
	if (Settings::Get()->GetDrawColliders())
	{
		SDL_Rect r{};
		Vector2f position = Camera::WorldToScreen(Vector2f(mOrigin.X - (Size.X / 2), mOrigin.Y + (Size.Y / 2)));
		r.x = (int)position.X;
		r.y = (int)position.Y;
		r.w = (int)(Size.X);
		r.h = (int)(Size.Y);

		SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(&renderer, &r);
	}
}

void BoundingBox::GetBoxAsPoints(Vector2f points[]) const
{
	points[0] = Vector2f(TopLeft.X, BottomRight.Y);
	points[1] = BottomRight;
	points[2] = Vector2f(BottomRight.X, TopLeft.Y);
	points[3] = TopLeft;
}

Vector2f BoundingBox::FindFurthestPoint(Vector2f direction) const
{
	Vector2f maxPoint;
	float maxDistance = -FLT_MAX;

	Vector2f vertices[4];
	GetBoxAsPoints(vertices);

	for (int i = 0; i < 4; i++)
	{
		float distance = vertices[i].Dot(direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = vertices[i];
		}
	}

	return maxPoint;
}
