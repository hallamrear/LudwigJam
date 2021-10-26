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
    //todo : remove
    std::vector<Projectile*> projs;

    void SetupInput();

    AnimationController* mAnimation;

    bool mFacingRight;
    void MoveRight();
    void MoveLeft();
    void Shoot();

public:
    Player(std::string texture, Transform transform, PhysicsProperties properties);
    ~Player() override;

    void Update(double deltaTime);
    void Render();
};

