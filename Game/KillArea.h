#pragma once
#include "Rigidbody.h"

class AnimationController;

class KillArea : public Rigidbody
{
private:

public:
	KillArea(Transform transform, int size);
	~KillArea();

	void OnOverlap(const CollisionManifold& manifold, Rigidbody& rb);
	void Update(double deltaTime);
	void Render();
};

