#pragma once
#include "Rigidbody.h"

class AnimationController;

class KillArea : public Rigidbody
{
private:
	AnimationController* mAnimation;

public:
	KillArea(Transform transform, Vector2f size);
	~KillArea();

	void OnOverlap(const CollisionManifold& manifold, Rigidbody& rb);
	void Update(double deltaTime);
	void Render();
};

