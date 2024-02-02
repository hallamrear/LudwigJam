#pragma once
#include <GameStates.h>

struct SDL_Renderer;
class Texture;
class TextElement;

class Startup :
    public GameState
{
private:
    bool mStarted;
    TextElement* mControlText[4];
    Texture* mControlTexture;

public:
    Startup();
    ~Startup();

    void Start();
    void End();
    void Update(double DeltaTime);
    void Render(SDL_Renderer& renderer);
};

