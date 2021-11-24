#pragma once
#include <Rigidbody.h>

class OrientedBoundingBox;
class Rigidbody;
class AnimationController;

class ForceArea :
    public Rigidbody
{
private:
    AnimationController* mAnimation;
    int mForceStrength;

public:
    ForceArea(Transform transform, int areaSize, int forceStrength);
    ~ForceArea();

    void ApplyForce(Rigidbody* rb);
    void Update(double deltaTime);
    void Render();

    virtual void OnOverlap(const CollisionManifold& manifold, Rigidbody& other) override;
    virtual void OnCollision(const CollisionManifold& manifold, Rigidbody& other) override;
    Collider* GetCollider();
};

