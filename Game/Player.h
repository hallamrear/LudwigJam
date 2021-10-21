#pragma once
#include <string>
#include "Character.h"

class Vector2f;
struct SDL_Renderer;
class Animation;

class Player :
    public Character
{
private:
    bool mFacingRight;
    Animation* mRunningRight;
    Animation* mRunningLeft;
    void MoveRight();
    void MoveLeft();
    void MoveDown();
    void MoveUp();

public:
    Player(std::string texture, Vector2f position, float rotation);
    ~Player() override;

    void SetupInput() override;
    void Update(double);
    void Render();
};

