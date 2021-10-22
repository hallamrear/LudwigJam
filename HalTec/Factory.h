#pragma once
#include "Entity.h"

enum class FACTORY_OBJECT_TYPE
{
	StaticWorldObject = 0,
	DynamicWorldObject,
	BackgroundObject
};

struct Transform;
struct PhysicsProperties;

class Factory
{
public:

	template <class T>
	 T* CreateObject(std::string texture, Transform transform, PhysicsProperties properties);
};

