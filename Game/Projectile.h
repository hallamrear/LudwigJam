#pragma once
#include "Rigidbody.h"

class AnimationController;

class Projectile : public Rigidbody
{
private:
	AnimationController* mAnimation;
public:
	Projectile(Transform transform);
	~Projectile();

	void Update(double deltaTime);
	void Render();
	void OnOverlap(const CollisionManifold& manifold, Rigidbody& other) override;
};

