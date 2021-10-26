#include "Settings.h"
#include "PlayState.h"
#include "BackgroundEntity.h"
#include "WorldObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "Player.h"
#include "Helper.h"
#include "Collision.h"
#include "Time.h"
#include "TextElement.h"
#include "ForceArea.h"
#include "BoundingBox.h"

#include "../HalTec/SDL2/include/SDL.h"

void PlayState::Start()
{
	Load("save.txt");

	Camera::SetCameraPosition(Vector2f(0.0f, -2000.0f));

	PlayAreaCollider = new BoundingBox(PlayAreaColliderTransform.Position, 5000.0f, 5000.0f);
	camLerp = true;
	mPlayer = new Player("", Transform(Vector2f(-550, -980.0f)), PhysicsProperties(150.0f, 0.1f, 1000.0f, 10.3f, true, true, false));
	entities.push_back(std::make_pair(2, mPlayer));
	Settings::Get()->SetDrawColliders(true);

	entities.push_back(std::make_pair(1, new TextElement(Transform())));
	mousePos = (TextElement*)entities.back().second;	

	entities.push_back(std::make_pair(1, new StaticWorldObject("Textures/Wall.bmp", Transform(Vector2f(620.0f, 0.0f), 180.0f))));
	entities.push_back(std::make_pair(1, new StaticWorldObject("Textures/Wall.bmp", Transform(Vector2f(-620.0f, 0.0f)))));
	entities.push_back(std::make_pair(1, new StaticWorldObject("Textures/Floor.bmp", Transform(Vector2f(0.0f, -1024.0f)))));
	entities.push_back(std::make_pair(1, new StaticWorldObject("Textures/Floor.bmp", Transform(Vector2f(0.0f, 1024.0f), 180.0f))));
	entities.push_back(std::make_pair(0, new BackgroundEntity("Textures/Test.bmp", Transform(Vector2f(60.0f, 50.0f)))));

	InputManager::Bind(IM_KEY_CODE::IM_KEY_0, IM_KEY_STATE::IM_KEY_PRESSED, [this] { camLerp = !camLerp; });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_RIGHT_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this] { Vector2f pos = Camera::GetCameraPosition(); pos.X += Time::DeltaTime() * 200.0f; Camera::SetCameraPosition(pos); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_LEFT_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this] { Vector2f pos = Camera::GetCameraPosition(); pos.X -= Time::DeltaTime() * 200.0f; Camera::SetCameraPosition(pos); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_UP_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this] { Vector2f pos = Camera::GetCameraPosition(); pos.Y += Time::DeltaTime() * 200.0f; Camera::SetCameraPosition(pos); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_DOWN_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this] { Vector2f pos = Camera::GetCameraPosition(); pos.Y -= Time::DeltaTime() * 200.0f; Camera::SetCameraPosition(pos); });

	PlacePlatforms();

	fOne = new ForceArea(Transform(Vector2f(-500.0f, -500.0f), 270.0f), Vector2f(250.0f, 250.0f), 16000.0f);
	fTwo = new ForceArea(Transform(Vector2f(500.0f, -500.0f), 180.0f), Vector2f(250.0f, 250.0f), 16000.0f);
	fThree = new ForceArea(Transform(Vector2f(-500.0f, 500.0f), 0.0f), Vector2f(250.0f, 250.0f), 16000.0f);
	fFour = new ForceArea(Transform(Vector2f(500.0f, 500.0f), 90.0f), Vector2f(250.0f, 250.0f), 16000.0f);
	entities.push_back(std::make_pair(1, fOne));
	entities.push_back(std::make_pair(1, fTwo));
	entities.push_back(std::make_pair(1, fThree));
	entities.push_back(std::make_pair(1, fFour));
}

void PlayState::End()
{
	Save("save.txt");

	delete mPlayer;
	mPlayer = nullptr;
}

void PlayState::PlacePlatforms()
{
	for (int y = -900.0f; y < 1000.0f; y += rand() % 200 + 50)
	{
		int x_count = rand() % 3 + 1;
		for (int x = 0; x < x_count; x++)
		{
			float x_pos = (float)(rand() % 1000 - 500);
			entities.push_back(std::make_pair(1, new StaticWorldObject("Textures/platform_small.bmp", Transform(Vector2f(x_pos, y)))));

		/*	int type = rand() % 3 + 1;
			switch (type)
			{
			case 1:
				break;
			case 2:
				entities.push_back(std::make_pair(1, new StaticWorldObject("Textures/platform_mid.bmp", Transform(Vector2f(x_pos, y)))));
				break;
			case 3:
				entities.push_back(std::make_pair(1, new StaticWorldObject("Textures/platform_large.bmp", Transform(Vector2f(x_pos, y)))));
				break;

			default:
				break;
			}*/


		}
	}

}

void PlayState::Save(std::string location)
{
}

void PlayState::Load(std::string location)
{
}

void PlayState::Update(double deltaTime)
{
	CollisionManifold m;
	if (!Collision::CheckCollision(*mPlayer->GetCollider(), *PlayAreaCollider, &m))
	{
		Vector2f vel = mPlayer->GetVelocity();
		mPlayer->AddVelocity(vel * -1.0f);
		mPlayer->GetTransform().Position = Vector2f();
	}


	std::string str = "";
	Vector2f pos = Camera::ScreenToWorld(InputManager::Get()->GetMousePosition());
	pos = Camera::Get()->GetCameraPosition();
	str = std::string("pX:" + std::to_string(pos.X) + " pY:" + std::to_string(pos.Y) + "\n");
	mousePos->SetString(str);
	mousePos->SetPosition(pos + Vector2f(0.0f, 55.0f));

	for (auto& one : entities)	
		one.second->Update(deltaTime);

	if (camLerp)
	{
		if (mPlayer->GetTransform().Position.Y < -685.0f)
		{
			Camera::SetCameraPosition(LerpPoint(Camera::GetCameraPosition(), Vector2f(0.0f, -685.0f), deltaTime));
		}
		else
		{
			Camera::SetCameraPosition(LerpPoint(Camera::GetCameraPosition(), Vector2f(0.0f, mPlayer->GetTransform().Position.Y), 5 * deltaTime));
		}
	}
		
	//Camera::SetCameraPosition(LerpPoint(Camera::GetCameraPosition(), Vector2f(mPlayer->GetTransform().Position), 5 * deltaTime));
}


void PlayState::Render(SDL_Renderer& renderer)
{
	for (int i = 0; i < 3; i++)
	{
		for (auto& itr : entities)
		{
			if(itr.first == i && itr.second->GetIsAlive())
				itr.second->Render();
		}
	}	
}