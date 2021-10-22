#pragma once
#include <string>
#include "Character.h"

class Vector2f;
struct SDL_Renderer;

class Player :
    public Character
{
private:
    bool mFacingRight;
    void MoveRight();
    void MoveLeft();
    void MoveDown();
    void MoveUp();

public:
    Player(std::string texture, Transform transform);
    ~Player() override;

    void SetupInput() override;
    void Update(double);
    void Render();
};

