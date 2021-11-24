#pragma once
#include <string>
#include "Rigidbody.h"

class AnimationController;
class Vector2f;
struct SDL_Renderer;
class Projectile;

class Player :
    public Rigidbody
{
private:
    void SetupInput();

    AnimationController* mAnimation;
    AnimationController* mJumpingAnimation;

    bool mFacingRight;
    void MoveRight();
    void MoveLeft();
    bool mIsJumping;

public:
    Player(std::string texture, Transform transform, PhysicsProperties properties);
    ~Player() override;

    void Update(double deltaTime) override;
    void Render() override;
};

