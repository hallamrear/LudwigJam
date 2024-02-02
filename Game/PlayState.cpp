#include "Settings.h"
#include "PlayState.h"
#include "BackgroundEntity.h"
#include "WorldObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "Player.h"
#include "Helper.h"
#include "Collision.h"
#include "MyTime.h"
#include "TextElement.h"
#include "ForceArea.h"
#include "BoundingBox.h"
#include "Texture.h"
#include "KillBox.h"
#include <iostream>
#include <fstream>
#include "OrientedBoundingBox.h"
#include "WinnerCoin.h"

#include "../HalTec/SDL2/include/SDL.h"

float r = 0.0f;

void PlayState::Start()
{
	resetPosition = Vector2f(-550, -1575.0f);

	buildMode = 0;
	Camera::SetCameraPosition(Vector2f(0.0f, -2000.0f));

	PlayAreaCollider = new BoundingBox(PlayAreaColliderTransform.Position, 5000.0f, 5000.0f);
	mouseEditCollider = new OrientedBoundingBox(mousePosition, r, 4.0f, 4.0f);

	camLerp = true;
	mPlayer = new Player("", Transform(resetPosition), PhysicsProperties(150.0f, 0.1f, 1000.0f, 10.3f, true, true, false));
	entities.push_back(std::make_pair(2, mPlayer));

	mWinnerCoin = new WinnerCoin(Transform(Vector2f(-450, -1610.0f)));
	entities.push_back(std::make_pair(2, mWinnerCoin));

	mousePos = new TextElement(Transform());

	modeString = new TextElement(Transform());

	Vector2f position = Vector2f(-5000.0f, -5000.0f);
	testSelections.push_back(new StaticWorldObject("Textures/platform_small.bmp", Transform(position)));
	testSelections.push_back(new StaticWorldObject("Textures/platform_mid.bmp", Transform(position)));
	testSelections.push_back(new StaticWorldObject("Textures/platform_large.bmp", Transform(position)));
	testSelections.push_back(new KillBox(Transform(position), 1));
	testSelections.push_back(new KillBox(Transform(position), 2));
	testSelections.push_back(new KillBox(Transform(position), 3));

	InputManager::Bind(IM_KEY_CODE::IM_KEY_Q, IM_KEY_STATE::IM_KEY_HELD, [this] { if(buildMode == 1) currentSelectionRotation--; else if (buildMode == 2) if (targetBody) targetBody->GetTransform().AdjustRotation(-1.0f); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_E, IM_KEY_STATE::IM_KEY_HELD, [this] { if (buildMode == 1) currentSelectionRotation++; else if (buildMode == 2) if (targetBody) targetBody->GetTransform().AdjustRotation(1.0f); });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_W, IM_KEY_STATE::IM_KEY_HELD, [this] { if (buildMode == 2) { if (targetBody) targetBody->GetTransform().AdjustPosition(Vector2f(0.0f, 1.0f)); } });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_S, IM_KEY_STATE::IM_KEY_HELD, [this] { if (buildMode == 2) { if (targetBody) targetBody->GetTransform().AdjustPosition(Vector2f(0.0f, -1.0f));} });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_A, IM_KEY_STATE::IM_KEY_HELD, [this] { if (buildMode == 2) { if (targetBody) targetBody->GetTransform().AdjustPosition(Vector2f(-1.0f, 0.0f));} });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_D, IM_KEY_STATE::IM_KEY_HELD, [this] { if (buildMode == 2) { if (targetBody) targetBody->GetTransform().AdjustPosition(Vector2f(1.0f, 0.0f)); } });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_4, IM_KEY_STATE::IM_KEY_PRESSED, [this] { if (buildMode == 2) { resetPosition = InputManager::Get()->GetMouseWorldPosition(); mPlayer->GetTransform().Position = resetPosition;  } });

	InputManager::Bind(IM_KEY_CODE::IM_KEY_8, IM_KEY_STATE::IM_KEY_PRESSED, [this] { if (buildMode == 2) { mWinnerCoin->GetTransform().Position = InputManager::Get()->GetMouseWorldPosition(); } });

	InputManager::Bind(IM_KEY_CODE::IM_KEY_0, IM_KEY_STATE::IM_KEY_PRESSED, [this] { camLerp = !camLerp; });
	InputManager::Bind(IM_KEY_CODE::IM_KEY_SPACE, IM_KEY_STATE::IM_KEY_PRESSED, [this] 
		{ 
			buildMode++;

			if (buildMode > 2)
				buildMode = 0;

			if (buildMode != 0)
			{
				Settings::Get()->SetDrawColliders(true);
			}
			else
				Settings::Get()->SetDrawColliders(false);
				
		});

	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this] 
		{ 
			switch (buildMode)
			{
			case 0:
				break;
			case 1:
			{
				Vector2f position = InputManager::Get()->GetMouseWorldPosition();
				switch (currentSelected)
				{
				case 1:
					entities.push_back({ 1, new StaticWorldObject("Textures/platform_small.bmp", Transform(position)) });
					break;

				case 2:
					entities.push_back({ 1, new StaticWorldObject("Textures/platform_mid.bmp", Transform(position)) });
					break;

				case 3:
					entities.push_back({ 1, new StaticWorldObject("Textures/platform_large.bmp", Transform(position)) });
					break;

				case 4:
					entities.push_back({ 1, new KillBox(Transform(position), 1) });
					break;

				case 5:
					entities.push_back({ 1, new KillBox(Transform(position), 2) });
					break;

				case 6:
					entities.push_back({ 1, new KillBox(Transform(position), 3) });
					break;
				}

				entities.back().second->GetTransform().Rotation = currentSelectionRotation;
			}
				break;
			case 2:
				
				if (targetBody != nullptr)
				{

				}

				break;
			default:
				break;
			}
		});

	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_RIGHT_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this] 
		{ 
			if (buildMode == 1 && targetBody != nullptr)
			{
				int location = -1;

				//Find target body in entities
				int size = entities.size();
				for (int i = 0; i < size; i++)
				{
					if (location != -1)
						break;

					if (entities[i].second == targetBody)
					{
						location = i;
					}
				}
				
				//Remove pair from entities
				entities.erase(entities.begin() + location);

				//Destroy entity.
				if (targetBody)
				{
					delete targetBody;
					targetBody = nullptr;
				}
			}	
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_1, IM_KEY_STATE::IM_KEY_PRESSED, [this]
		{
			testSelections[currentSelected - 1]->GetTransform().Position = Vector2f(-5000.0f, -5000.0f);
			currentSelected++;

			if (currentSelected == 7)
				currentSelected = 1;
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_2, IM_KEY_STATE::IM_KEY_PRESSED, [this]
		{
			testSelections[currentSelected - 1]->GetTransform().Position = Vector2f(-5000.0f, -5000.0f);
			currentSelected--;

			if (currentSelected < 1)
				currentSelected = 6;
		}); 

	InputManager::Bind(IM_KEY_CODE::IM_KEY_RIGHT_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this] 
		{ 
			Vector2f pos = Camera::GetCameraPosition();
			pos.X += Time::DeltaTime() * 200.0f;
			Camera::SetCameraPosition(pos); 
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_LEFT_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this]
		{ 
			Vector2f pos = Camera::GetCameraPosition();
			pos.X -= Time::DeltaTime() * 200.0f;
			Camera::SetCameraPosition(pos); 
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_UP_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this]
		{ 
			Vector2f pos = Camera::GetCameraPosition();
			pos.Y += Time::DeltaTime() * 200.0f; 
			Camera::SetCameraPosition(pos);
		});

	InputManager::Bind(IM_KEY_CODE::IM_KEY_DOWN_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this]
		{ 
			Vector2f pos = Camera::GetCameraPosition(); 
			pos.Y -= Time::DeltaTime() * 200.0f;
			Camera::SetCameraPosition(pos);
		});

	//fOne = new ForceArea(Transform(Vector2f(-390.0f, -1580.0f), 0.0f), 1, 16000.0f);
	//fTwo =   new ForceArea(Transform(Vector2f(-200.0f, -1580.0f), 0.0f), 2, 16000.0f);
	//fThree = new ForceArea(Transform(Vector2f(-100.0f, -1580.0f), 0.0f), 3, 16000.0f);
	//entities.push_back(std::make_pair(1, fOne));
	//entities.push_back(std::make_pair(1, fTwo));
	//entities.push_back(std::make_pair(1, fThree));

	//fFour = new ForceArea(Transform(Vector2f(500.0f, 500.0f), 90.0f), Vector2f(250.0f, 250.0f), 16000.0f);
	//entities.push_back(std::make_pair(1, fFour));

	Camera::SetCameraPosition(LerpPoint(Camera::GetCameraPosition(), Vector2f(0.0f, -1285.0f), 1));

	Load("save.txt");
}

void PlayState::End()
{
	if (mWinnerCoin->HasBeenCollected())
	{
		mPlayer->GetTransform().Position = resetPosition;
	}

	Save("save.txt");
}

void PlayState::Save(std::string location)
{
	std::fstream stream;
	stream.open(location, std::fstream::out | std::fstream::trunc);

	if (stream.is_open())
	{
		stream << std::to_string(mPlayer->GetTransform().Position.X);
		stream << '\n';
		stream << std::to_string(mPlayer->GetTransform().Position.Y);
		stream << '\n';
		
		int count = 0;
		std::vector<std::pair<int, Entity*>> typeAndTransform;

		int size = entities.size();
		for (int i = 0; i < size; i++)
		{
			Entity* entity = entities[i].second;

			if (dynamic_cast<StaticWorldObject*>(entity))
			{
				typeAndTransform.push_back({ 1, entity });
				count++;
			}
			else if (dynamic_cast<KillBox*>(entity))
			{
				typeAndTransform.push_back({ 2, entity });
				count++;
			}
			else if (dynamic_cast<WinnerCoin*>(entity))
			{
				typeAndTransform.push_back({ 3, entity });
				count++;
			}
		}

		stream << count;
		stream << '\n';

		for (int j = 0; j < count; j++)
		{
			Transform t = typeAndTransform[j].second->GetTransform();

			stream << typeAndTransform[j].first;
			stream << '\n';
			stream << std::to_string(t.Position.X);
			stream << '\n';
			stream << std::to_string(t.Position.Y);
			stream << '\n';
			stream << std::to_string(t.Rotation);
			stream << '\n';
			stream << typeAndTransform[j].second->GetTexture().GetLocation();
			stream << '\n';
		}
	}

	stream.close();
}

void PlayState::Load(std::string location)
{
	std::fstream stream;
	stream.open(location, std::fstream::in);

	if (stream.is_open())
	{
		std::string location = "";
		int count = -1;
		int type = -1;
		Transform transform;
		stream >> transform.Position.X;
		stream >> transform.Position.Y;
		mPlayer->GetTransform().Position = transform.Position;
		transform = Transform();
		stream >> count;

		for (int i = 0; i < count; i++)
		{
			stream >> type;
			stream >> transform.Position.X;
			stream >> transform.Position.Y;
			stream >> transform.Rotation;
			stream >> location;

			switch (type)
			{
			case 1: //General static object
			{
				entities.push_back({ 1, new StaticWorldObject(location, transform) });
			}
			break;

			case 2:	//Kill box
			{
				if (location == "Textures/killbox-large.bmp")
					entities.push_back({ 1, new KillBox(transform, 3) });
				else if (location == "Textures/killbox-mid.bmp")
					entities.push_back({ 1, new KillBox(transform, 2) });
				else if (location == "Textures/killbox-small.bmp")
					entities.push_back({ 1, new KillBox(transform, 1) });
			}
			break;

			case 3:
			{
				mWinnerCoin->GetTransform() = transform;
			}
			break;

			default:
			{
				std::cout << "Tried to load unsupported entity with type id {" << type <<  "}" << std::endl;
			}
				break;
			}

		}
	}

	stream.close();
}

void PlayState::Update(double deltaTime)
{
	targetBody = nullptr;

	switch (buildMode)
	{
	case 0:
		modeString->SetString(" ");
		testSelections[currentSelected - 1]->GetTransform().Position = Vector2f(-5000.0f, -5000.0f);
		testSelections[currentSelected - 1]->GetTransform().Rotation = 0.0f;
		break;
	case 1:
		modeString->SetString("Add/Delete");
		testSelections[currentSelected - 1]->GetTransform().Position = InputManager::Get()->GetMouseWorldPosition();
		testSelections[currentSelected - 1]->GetTransform().Rotation = currentSelectionRotation;
		break;
	case 2: 
		modeString->SetString("Edit");
		testSelections[currentSelected - 1]->GetTransform().Position = Vector2f(-5000.0f, -5000.0f);
		testSelections[currentSelected - 1]->GetTransform().Rotation = 0.0f;
		break;
	}

	if (buildMode != 0)
	{
		mouseEditCollider->mOrigin = InputManager::Get()->GetMouseWorldPosition();
		mouseEditCollider->Update(deltaTime);

		modeString->SetPosition(InputManager::Get()->GetMouseWorldPosition() + Vector2f(0.0f, -100.0f));
		modeString->Update(deltaTime);

		std::string str = "";
		Vector2f pos = InputManager::Get()->GetMouseWorldPosition();
		str = std::string("pX:" + std::to_string(pos.X) + " pY:" + std::to_string(pos.Y) + "\n");
		mousePos->SetString(str);
		mousePos->SetPosition(pos + Vector2f(0.0f, 55.0f));
		mousePos->Update(deltaTime);
	}
	else
	{
		mouseEditCollider->mOrigin = Vector2f(5000.0f, 5000.0f);

		if (camLerp)
		{
			if (mPlayer->GetTransform().Position.Y < -1285.0f)
			{
				Camera::SetCameraPosition(LerpPoint(Camera::GetCameraPosition(), Vector2f(0.0f, -1285.0f), 5 * deltaTime));
			}
			else
			{
				Camera::SetCameraPosition(LerpPoint(Camera::GetCameraPosition(), Vector2f(0.0f, mPlayer->GetTransform().Position.Y), 5 * deltaTime));
			}
		}
	}

	CollisionManifold m;
	if (!Collision::CheckCollision(*mPlayer->GetCollider(), *PlayAreaCollider, &m))
	{
		Vector2f vel = mPlayer->GetVelocity();
		mPlayer->AddVelocity(vel * -1.0f);
		mPlayer->GetTransform().Position = resetPosition;
	}

	if (!mPlayer->GetIsAlive())
	{
		Vector2f vel = mPlayer->GetVelocity();
		mPlayer->AddVelocity(vel * -1.0f);
		mPlayer->GetTransform().Position = resetPosition;
		mPlayer->SetAlive(true);
	}

	for (auto& one : entities)
	{
		one.second->Update(deltaTime);

		if (buildMode != 0)
		{
			Rigidbody* rb = dynamic_cast<Rigidbody*>(one.second);

			if (rb != nullptr)
			{
				if (Collision::CheckCollision(*mouseEditCollider, *rb->GetCollider(), &m))
				{
					targetBody = one.second;
				}
			}
			else
				targetBody = nullptr;
		}
	}

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

	if (buildMode != 0)
	{
		mousePos->Render();
		modeString->Render();
		testSelections[currentSelected - 1]->Render();

		if (buildMode == 2)
			mouseEditCollider->Render(renderer);
	}
}