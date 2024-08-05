#pragma once
#include "Vector2.h"
#include "List.h"

enum EOBJECT_TYPE
{
	PLAYER,
	ENEMY,
	PLAYER_BULLET,
	ENEMY_BULLET,
};

#define OBJECT_RADIUS 30.0f
#define BULLET_RADIUS 10.0f

class Object
{
public:
	EOBJECT_TYPE m_type = PLAYER;
	List<Object>* m_pBulletContainer = nullptr;

	Vector2	m_position = { 0.0f,0.0f };
	float m_speedPerSec = 500.0f;
		
	Vector2 m_inputVector;
	
	float m_radius = OBJECT_RADIUS;

	void Initialize(EOBJECT_TYPE type);
	void UpdatePosition(float elapsedTimeSec);
	void Render();
	bool IsCollide(const Object& other);
};

