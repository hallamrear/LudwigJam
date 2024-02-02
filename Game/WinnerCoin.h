#pragma once
#include <Rigidbody.h>

class AnimationController;

class WinnerCoin :
    public Rigidbody
{
private:
	AnimationController* mCoinAnimation;
	bool mIsCollected;

public:
	WinnerCoin(Transform transform);
	~WinnerCoin();

	void OnOverlap(const CollisionManifold& manifold, Rigidbody& rb);
	void Update(double deltaTime);
	void Render();

	const bool& HasBeenCollected() const;
};