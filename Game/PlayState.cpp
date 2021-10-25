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

	mPlayer = new Player("", Transform(Vector2f(0.0f, 100.0f)));
	entities.push_back(mPlayer);
	Settings::Get()->SetDrawColliders(true);

	manifold = new CollisionManifold();
	hitManifold = new CollisionManifold();

	entities.push_back(new StaticWorldObject("Textures/Floor2.bmp", Transform(Vector2f(0.0f, -500.0f))));
	entities.push_back(new StaticWorldObject("Textures/Floor2.bmp", Transform(Vector2f(0.0f, 700.0f))));
	entities.push_back(new StaticWorldObject("Textures/Floor2.bmp", Transform(Vector2f(700.0f, 0.0f), 90.0f)));
	entities.push_back(new StaticWorldObject("Textures/Floor2.bmp", Transform(Vector2f(-700.0f, 0.0f), 90.0f)));
	entities.push_back(new StaticWorldObject("Textures/Floor.bmp", Transform(Vector2f(-200.0f, -100.0f))));
	entities.push_back(new StaticWorldObject("Textures/Floor.bmp", Transform(Vector2f(400.0f, 250.0f))));
	entities.push_back(new BackgroundEntity("Textures/Test.bmp", Transform(Vector2f(60.0f, 50.0f))));
	entities.push_back(new TestEntity(0, "Textures/Test2.bmp", Transform(Vector2f(-300.0f, 250.0f))));
	entities.push_back(new TestEntity(1, "Textures/Test2.bmp", Transform(Vector2f(-200.0f, 250.0f))));	
	entities.push_back(new TestEntity(2, "Textures/Test2.bmp", Transform(Vector2f(-100.0f, 250.0f))));

	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this] 
	{ 
		Vector2f position = Camera::ScreenToWorld(InputManager::Get()->GetMousePosition());
		entities.push_back(new TestEntity(2, "Textures/Test2.bmp", Transform(position)));
	});

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
	for (auto& one : entities)
	{
		for (auto& two : entities)
		{
			if (one == two)
				continue;			

			if (one->GetCollider() && two->GetCollider())
			{
				if (Collision::CheckCollision(*one->GetCollider(), *two->GetCollider(), manifold))
				{
					if (two->GetCollider()->mType == COLLIDER_TYPE::COLLIDER_SPHERE)
					{
						a = one;
						b = two;
						hitManifold->HasCollided = manifold->HasCollided;
						hitManifold->Depth = manifold->Depth;
						hitManifold->Normal = manifold->Normal;
					}
					
					Collision::ResolveCollision(*one, *two, manifold);
				}
			}
		}

		one->Update(deltaTime);
	}

	Entity* target = entities[10];

	Vector2f pos = target->GetTransform().Position;
	std::string str = "Pos -> X: " + std::to_string(target->GetTransform().Position.X) + ", Y: " + std::to_string(target->GetTransform().Position.Y);
	posText->SetString(str);
	posText->SetPosition(pos + Vector2f(0.0f, 225.0f));
	posText->Update(deltaTime);

	str = "Vel -> X: " + std::to_string(target->GetVelocity().X) + ", Y: " + std::to_string(target->GetVelocity().Y);
	velText->SetString(str);
	velText->SetPosition(pos + Vector2f(0.0f, 250.0f));
	velText->Update(deltaTime);

	str = "Acc -> X: " + std::to_string(target->GetAcceleration().X) + ", Y: " + std::to_string(target->GetAcceleration().Y);
	accText->SetString(str);
	accText->SetPosition(pos + Vector2f(0.0f, 275.0f));
	accText->Update(deltaTime);

	Camera::SetCameraPosition(LerpPoint(Camera::GetCameraPosition(), mPlayer->GetTransform().Position, 5 * deltaTime));
}

void PlayState::Render(SDL_Renderer& renderer)
{
	posText->Render();
	velText->Render();
	accText->Render();

	for (auto& itr : entities)
		itr->Render();

	if (a && b && hitManifold->HasCollided)
	{
		Vector2f p1, p2;
		p1 = b->GetTransform().Position;
		p2 = p1 + (hitManifold->Normal * hitManifold->Depth);
		p1 = Camera::WorldToScreen(p1);
		p2 = Camera::WorldToScreen(p2);
		SDL_RenderDrawLine(&renderer, p1.X, p1.Y, p2.X, p2.Y);
	}	
}