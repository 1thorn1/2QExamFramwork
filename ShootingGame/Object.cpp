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
	// EXAM_03. �ڽŰ� �ٸ� ������Ʈ�� ��ġ�� �������� ����Ͽ� ���� ���� �浹�˻� �Լ��� �ۼ��Ͻÿ�
	float distance = sqrt(pow(this->m_position.x - other.m_position.x, 2) + pow(this->m_position.y - other.m_position.y, 2));
	if (distance <= this->m_radius + other.m_radius)
	{
		return true;
	}
	////////////////////////
	return false;
}

void Object::UpdatePosition(float elapsedTimeSec)
{
	Vector2 moveDirection(0.0f, 0.0f);
	if (m_inputVector != Vector2(0.0f, 0.0f))
	{
		m_inputVector.Normalize();
		moveDirection = m_inputVector;
	}
	/////////////////////////
	// EXAM_04 �̵������ �ӷ� �׸��� ����ð��� ����Ͽ� ��ġ�� ������Ʈ�ϴ� �ڵ带 �ۼ��Ͻÿ�
	m_position += moveDirection * m_speedPerSec * elapsedTimeSec;
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
	Render::DrawCircle((int)m_position.x, (int)m_position.y, (int)m_radius, color);
}








