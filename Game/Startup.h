#pragma once
#include <GameStates.h>

struct SDL_Renderer;
class Texture;
class TextElement;

class Startup :
    public GameState
{
private:
    TextElement** controlText;

    TextElement* text;
    bool mStarted;
    Texture* mTexture;

public:
    void Start();
    void End();
    void Update(double DeltaTime);
    void Render(SDL_Renderer& renderer);
};

