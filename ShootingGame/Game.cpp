#include "stdafx.h"
#include "Game.h"

#include "System.h"
#include "Object.h"
#include "List.h"
#include <random>

namespace Game
{
	const int MAX_ENEMY = 10;
	Object g_player;

	int g_Score = 0;
	float g_SpawnTime = 0.0f;	
	float g_SpawnInterval = 1.0f;

	float g_resetPlayerTime = 0.0f;

	float g_FireTime = 0.0f;
	float g_FireInterval = 1.0f;
	
	random_device rd;
	mt19937 gen(rd());
	
	List<Object> g_EnemyContainer;
	List<Object> g_EnemyBulletContainer;
	List<Object> g_PlayerBulletContainer;

	void SpawnEnemy()
	{		
		std::uniform_int_distribution<> x(0, Render::GetScreenSize().cx);
		std::uniform_int_distribution<> y(0, Render::GetScreenSize().cy);

		Object enemy;
		enemy.Initialize(EOBJECT_TYPE::ENEMY);
		enemy.m_position = Vector2((float)(x(gen)), (float)(y(gen)));
		enemy.m_inputVector = g_player.m_position - enemy.m_position;
		enemy.m_speedPerSec = 80.0f;

		g_EnemyContainer.push_back(enemy);
	}
	void ResetPlayer()
	{
		g_player.m_position = Vector2((float)Render::GetScreenSize().cx / 2, (float)Render::GetScreenSize().cy / 2);
		g_Score = 0;
	}
	void RestartPlayer(float elapsedTime)
	{
		g_player.Initialize(EOBJECT_TYPE::PLAYER);
		g_resetPlayerTime += elapsedTime;
		if (g_resetPlayerTime > 0.5f)
		{
			g_resetPlayerTime = 0.0f;
			g_player.m_position = Vector2((float)Render::GetScreenSize().cx / 2, (float)Render::GetScreenSize().cy / 2);
			g_Score = 0;
		}
	}
	void Initialize(HWND hwnd,int witdh,int height)
	{
		Render::Initialize(hwnd, witdh, height);
		Input::Initialize(hwnd, witdh, height);
		Time::Initialize();

		g_player.Initialize(EOBJECT_TYPE::PLAYER);
		ResetPlayer();
	}

	void ProcessPlayerInput()
	{
		g_player.m_inputVector.Set(0.0f, 0.0f);
		if (Input::IsCurrDn(VK_LEFT))
		{
			g_player.m_inputVector.x = -1;
		}
		else if (Input::IsCurrDn(VK_RIGHT))
		{
			g_player.m_inputVector.x = 1;
		}
		if (Input::IsCurrDn(VK_UP))
		{
			g_player.m_inputVector.y = -1;
		}
		else if (Input::IsCurrDn(VK_DOWN))
		{
			g_player.m_inputVector.y = 1;
		}
		if (Input::IsTurnDn(VK_SPACE))
		{
			//ÃÑ¾Ë ¹ß»ç
			Object bullet;
			bullet.Initialize(EOBJECT_TYPE::PLAYER_BULLET);
			bullet.m_position = g_player.m_position;
			bullet.m_inputVector.Set(0, -1);
			g_PlayerBulletContainer.push_back(bullet);
		}
	}
	void ProcessSpawn(float elapsedTime)
	{
		std::uniform_int_distribution<> r(0, 1);
		g_SpawnTime += elapsedTime;
		if (g_SpawnTime > g_SpawnInterval)
		{
			g_SpawnTime = 0.0f;
			if (g_EnemyContainer.size() < 3)
			{
				SpawnEnemy();
			}
		}
		g_FireTime += elapsedTime;
		if (g_FireTime > g_FireInterval)
		{
			g_FireTime = 0.0f;
			List<Object>::Node* pNode = nullptr;
			pNode = g_EnemyContainer.m_pHead;

			while (pNode != nullptr)
			{
				if (r(gen) == 0)
				{
					Object bullet;
					bullet.Initialize(EOBJECT_TYPE::ENEMY_BULLET);
					bullet.m_position = pNode->data.m_position;
					bullet.m_inputVector = g_player.m_position - bullet.m_position;
					g_EnemyBulletContainer.push_back(bullet);
				}
				pNode = pNode->pNext;
			}
		}
	}

	void ProcessObjectPosition(float elapsedTime)
	{
		std::uniform_int_distribution<> v(-1, 1);
		g_player.UpdatePosition(elapsedTime);
		if (!Render::IsInScreen((int)g_player.m_position.x, (int)g_player.m_position.y))
		{
			g_player.m_position.x = std::clamp(g_player.m_position.x, 0.0f, (float)Render::GetScreenSize().cx);
			g_player.m_position.y = std::clamp(g_player.m_position.y, 0.0f, (float)Render::GetScreenSize().cy);
		}

		List<Object>::Node* pNode = nullptr;
		List<Object>::Node* pNodeNext = nullptr;
		pNode = g_EnemyContainer.m_pHead;
		while (pNode != nullptr)
		{
			pNodeNext = pNode->pNext;
			pNode->data.UpdatePosition(elapsedTime);
			if (!Render::IsInScreen((int)pNode->data.m_position.x, (int)pNode->data.m_position.y))
			{
				pNode->data.m_inputVector.x = (float)(v(gen));
				pNode->data.m_inputVector.y = (float)(v(gen));
			}
			pNode = pNodeNext;
		}

		pNode = g_PlayerBulletContainer.m_pHead;
		pNodeNext = nullptr;
		while (pNode != nullptr)
		{
			pNodeNext = pNode->pNext;
			pNode->data.UpdatePosition(elapsedTime);
			if (!Render::IsInScreen((int)pNode->data.m_position.x, (int)pNode->data.m_position.y))
			{
				g_PlayerBulletContainer.erase(pNode);
			}
			pNode = pNodeNext;
		}

		pNode = g_EnemyBulletContainer.m_pHead;
		while (pNode != nullptr)
		{
			pNodeNext = pNode->pNext;
			pNode->data.UpdatePosition(elapsedTime);
			if (!Render::IsInScreen((int)pNode->data.m_position.x, (int)pNode->data.m_position.y))
			{
				g_EnemyBulletContainer.erase(pNode);
			}
			pNode = pNodeNext;
		}
	}
	void ProcessObjectCollision(float elapsedTime)//
	{
		List<Object>::Node* pNode = nullptr;
		List<Object>::Node* pNodeNext = nullptr;
		pNode = g_PlayerBulletContainer.m_pHead;
		while (pNode != nullptr)
		{
			pNodeNext = pNode->pNext;
			List<Object>::Node* pEnemyNode = nullptr;
			pEnemyNode = g_EnemyContainer.m_pHead;
			while (pEnemyNode != nullptr)
			{
				if (pNode->data.IsCollide(pEnemyNode->data))
				{
					g_PlayerBulletContainer.erase(pNode);
					g_EnemyContainer.erase(pEnemyNode);
					g_Score++;
					break;
				}
				pEnemyNode = pEnemyNode->pNext;
			}
			pNode = pNodeNext;
		}

		pNode = g_EnemyBulletContainer.m_pHead;
		while (pNode != nullptr)
		{
			pNodeNext = pNode->pNext;
			if (g_player.IsCollide(pNode->data))
			{
				g_EnemyBulletContainer.erase(pNode);
				ResetPlayer();
				break;
			}
			pNode = pNodeNext;
		}
	
		pNode = g_EnemyContainer.m_pHead;
		while (pNode != nullptr)
		{
			pNodeNext = pNode->pNext;
			if (g_player.IsCollide(pNode->data))
			{
				g_EnemyContainer.erase(pNode);
				
				ResetPlayer();
				break;
			}
			pNode = pNodeNext;
		}
	}

	void Update()
	{
		Time::UpdateTime();
		float elapsedTime = Time::GetElapsedTime();
		Input::Update();

		ProcessPlayerInput();
		ProcessSpawn(elapsedTime);
		ProcessObjectPosition(elapsedTime);
		ProcessObjectCollision(elapsedTime); //
	}
	void Render()
	{
		Render::BeginDraw();

		g_player.Render();
		List<Object>::Node* pNode = nullptr;
		pNode = g_PlayerBulletContainer.m_pHead;
		while (pNode!=nullptr)
		{
			pNode->data.Render();
			pNode = pNode->pNext;
		}

		pNode = g_EnemyContainer.m_pHead;
		while (pNode != nullptr)
		{
			pNode->data.Render();
			pNode = pNode->pNext;
		}

		pNode = g_EnemyBulletContainer.m_pHead;
		while (pNode != nullptr)
		{
			pNode->data.Render();
			pNode = pNode->pNext;
		}

		
		Render::DrawText(Render::GetScreenSize().cx / 2-20, Render::GetScreenSize().cy-100, "SCORE",RGB(255,0,0));

		
		char szScore[256]={0,};
		sprintf_s(szScore, "%d", g_Score);
		Render::DrawText(Render::GetScreenSize().cx / 2+40, Render::GetScreenSize().cy - 100, szScore, RGB(255, 0, 0));


		Render::EndDraw();
	}


	void Uninitialize()
	{
		g_EnemyBulletContainer.clear();		
		g_EnemyContainer.clear();
		g_PlayerBulletContainer.clear();

		Input::Uninitialize();
		Render::Uninitialize();
	}


}