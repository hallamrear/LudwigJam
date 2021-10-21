#pragma once
#include <Entity.h>

class TestEntity :
    public Entity
{

public:
    TestEntity(int testMode, std::string texture, Vector2f position, float rotation);
    ~TestEntity();

    void Update(double deltaTime);
    void Render();
};

