#include "Character.h"
#include "Game.h"
#include "Animation.h"

Character::Character(std::string texture, Vector2f position, float rotation)
	: Entity(texture, position, rotation, 0.0f, 0.0f, 0.0f)
{
	
}

Character::~Character()
{

}
