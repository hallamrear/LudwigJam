#pragma once
#include <Entity.h>

class TestEntity :
    public Entity
{

public:
    TestEntity(int testmode, std::string texture, Transform transform);
    ~TestEntity();

    void Update(double deltaTime);
    void Render();
};

