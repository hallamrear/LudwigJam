#include "Character.h"
#include "Game.h"
#include "Animation.h"

Character::Character(std::string texture, Transform transform, bool isStatic)
	: Entity(texture, transform, isStatic)
{
	
}

Character::~Character()
{

}
