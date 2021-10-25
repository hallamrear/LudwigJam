#include "Character.h"
#include "Game.h"
#include "Animation.h"

Character::Character(std::string texture, Transform transform, bool isStatic)
	: Entity(texture, transform, isStatic, 200.0f, 0.5f, 500.0f, 0.0f)
{	
}

Character::~Character()
{

}
