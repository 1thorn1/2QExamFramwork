#include "stdafx.h"
#include "Object.h"
#include "System.h"


void Object::Initialize(EOBJECT_TYPE type)
{
	m_type = type;	
	if (m_type == PLAYER_BULLET || m_type == ENEMY_BULLET)
	{
		m_radius = BULLET_RADIUS;
	}
}

bool Object::IsCollide(const Object& other)
{
	///////	/////////////////
	// EXAM_03. 자신과 다른 오브젝트의 위치와 반지름을 고려하여 원과 원의 충돌검사 함수를 작성하시오

	////////////////////////
	return false;
}

void Object::UpdatePosition(float elapsedTimeSec)
{	
	Vector2 moveDirection(0.0f,0.0f);
	if (m_inputVector != Vector2(0.0f, 0.0f))
	{
		m_inputVector.Normalize();
		moveDirection = m_inputVector;
	}	
	/////////////////////////
	// EXAM_04 이동방향과 속력 그리고 경과시간을 고려하여 위치를 업데이트하는 코드를 작성하시오

	////////////////////////
}

void Object::Render()
{		
	COLORREF color = RGB(255, 255, 255);
	switch (m_type)
	{
	case PLAYER:
	case PLAYER_BULLET:
		color = RGB(0, 0, 255);
		break;
	case ENEMY:
	case ENEMY_BULLET:
		color = RGB(255, 0, 0);
		break;
	}
	Render::DrawCircle((int)m_position.x, (int)m_position.y,(int)m_radius, color);
}








