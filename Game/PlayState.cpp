#include "Settings.h"
#include "PlayState.h"
#include "TestEntity.h"
#include "BackgroundEntity.h"
#include "WorldObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "Player.h"
#include "Helper.h"
#include "Collision.h"
#include "Time.h"
#include "TextElement.h"



#include "../HalTec/SDL2/include/SDL.h"

void PlayState::Start()
{
	posText = new TextElement(Vector2f(100.0f, -200.0f));
	velText = new TextElement(Vector2f(100.0f, -220.0f));
	accText = new TextElement(Vector2f(100.0f, -240.0f));
	collisionText = new TextElement(Vector2f(100.0f, -320.0f));

	mPlayer = new Player("", Vector2f(), 0.0f);

	Settings::Get()->SetDrawColliders(true);

	entities.push_back(new TestEntity(0, "Textures/Floor.bmp", Vector2f(1.0f, 60.0f), 0.0f));
	entities.push_back(new BackgroundEntity("Textures/Test.bmp", Vector2f(1.0f, 50.0f), 0.0f));
	entities.push_back(new TestEntity(0, "Textures/Test2.bmp", Vector2f(-150.0f, 250.0f), 0.0f));
	entities.push_back(new TestEntity(1, "Textures/Test2.bmp", Vector2f(0.0f, 250.0f), 0.0f));
	entities.push_back(new TestEntity(2, "Textures/Test2.bmp", Vector2f(150.0f, 250.0f), 0.0f));

	InputManager::Bind(IM_KEY_CODE::IM_KEY_RIGHT_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this] { Vector2f pos = Camera::GetCameraPosition(); pos.X += Time::DeltaTime() * 200.0f; Camera::SetCameraPosition(pos); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_LEFT_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this] { Vector2f pos = Camera::GetCameraPosition(); pos.X -= Time::DeltaTime() * 200.0f; Camera::SetCameraPosition(pos); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_UP_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this] { Vector2f pos = Camera::GetCameraPosition(); pos.Y += Time::DeltaTime() * 200.0f; Camera::SetCameraPosition(pos); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_DOWN_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this] { Vector2f pos = Camera::GetCameraPosition(); pos.Y -= Time::DeltaTime() * 200.0f; Camera::SetCameraPosition(pos); });
}

void PlayState::End()
{
	delete mPlayer;
	mPlayer = nullptr;
}

void PlayState::Update(double deltaTime)
{
	CollisionManifold manifold;
	if (Collision_Detection::CheckCollision(*mPlayer->GetCollider(), *entities[0]->GetCollider(), &manifold))
		collisionText->SetString("collision");
	else
		collisionText->SetString("no collision");

	for (auto& itr : entities)
	{
		itr->Update(deltaTime);
	}

	Vector2f pos = mPlayer->GetPosition();
	std::string str = "Pos -> X: " + std::to_string(mPlayer->GetPosition().X) + ", Y: " + std::to_string(mPlayer->GetPosition().Y);
	posText->SetString(str);
	posText->SetPosition(pos + Vector2f(0.0f, -10.0f));
	posText->Update(deltaTime);

	str = "Vel -> X: " + std::to_string(mPlayer->GetVelocity().X) + ", Y: " + std::to_string(mPlayer->GetVelocity().Y);
	velText->SetString(str);
	velText->SetPosition(pos + Vector2f(0.0f, -30.0f));
	velText->Update(deltaTime);

	str = "Acc -> X: " + std::to_string(mPlayer->GetAcceleration().X) + ", Y: " + std::to_string(mPlayer->GetAcceleration().Y);
	accText->SetString(str);
	accText->SetPosition(pos + Vector2f(0.0f, -50.0f));
	accText->Update(deltaTime);

	collisionText->SetPosition(pos + Vector2f(0.0f, -70.0f));
	collisionText->Update(deltaTime);

	mPlayer->Update(deltaTime);

	Camera::SetCameraPosition(LerpPoint(Camera::GetCameraPosition(), mPlayer->GetPosition(), 2 * deltaTime));
}

void PlayState::Render(SDL_Renderer& renderer)
{
	posText->Render();
	velText->Render();
	accText->Render();
	collisionText->Render();

	for (auto& itr : entities)
		itr->Render();

	mPlayer->Render();
}