#pragma once

struct SDL_Texture;

class Texture
{
protected:
	SDL_Texture* mTexture;
	//Creates the sdl texture and returns the address that will be stored in the map.
	void Create(std::string texture_path);

public:
	Texture(std::string texture_path);
	~Texture();

	int Width;
	int Height;
	SDL_Texture& GetSDLTexture();

	virtual void Render(SDL_Renderer& renderer, Vector2f position, float rotation, Vector2f sourcePosition, Vector2f sourceDimensions);
	virtual void Render(SDL_Renderer& renderer, Vector2f position, float rotation);
};


