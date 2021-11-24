#pragma once
#include "Rigidbody.h"

class AnimationController;

class KillBox : public Rigidbody
{
private:

public:
	KillBox(Transform transform, int size);
	~KillBox();

	void OnOverlap(const CollisionManifold& manifold, Rigidbody& rb);
	void Update(double deltaTime);
	void Render();
};

