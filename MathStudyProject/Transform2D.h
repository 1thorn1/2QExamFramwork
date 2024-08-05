/**
	@file      Transform2D.h
	@brief     간단한 2D Transform 처리를 위한 클래스 입니다.
	@author    SKYFISH
	@date      JUNE.2023
	@notice    52반 수업을 위한 테스트용 코드이며 수정 중에 있습니다.
**/

#pragma once

#include "SimpleMathHelper.h"
//
// 교육용 2D 기반 Transform
// 

class Transform2D
{
public:
	Transform2D() = default;

	Transform2D(const Transform2D&) = default;

	Transform2D& operator=(const Transform2D&) = default;

	Transform2D(Transform2D&&) = default;

	Transform2D& operator=(Transform2D&&) = default;

	~Transform2D() = default;

	void SetPosition(float x, float y)
	{
		m_position.x = x;
		m_position.y = y;
	}

	void SetPosition(const mathHelper::Vector2F& position)
	{
		m_position = position;

	}

	void SetPositionX(float x)
	{
		m_position.x = x;

	}

	void SetPositionY(float y)
	{
		m_position.y = y;

	}

	void SetScale(float x, float y)
	{
		m_scale.x = x;
		m_scale.y = y;
	}

	void SetScale(const mathHelper::Vector2F& scale)
	{
		m_scale = scale;

	}

	void SetScaleX(float x)
	{
		m_scale.x = x;

	}

	void SetScaleY(float y)
	{
		m_scale.y = y;

	}
	void SetRotation(float rotation)
	{
		m_rotation = rotation;

	}

	void SetRotationCenter(float x, float y)
	{
		m_rotationCenter.x = x;
		m_rotationCenter.y = y;
	}

	void SetRotationCenter(const mathHelper::Vector2F& rotationCenter)
	{
		m_rotationCenter = rotationCenter;
	}

	void SetRotationCenterX(float x)
	{
		m_rotationCenter.x = x;

	}

	void SetRotationCenterY(float y)
	{
		m_rotationCenter.y = y;

	}

	const mathHelper::Vector2F& GetPosition() const
	{
		return m_position;
	}

	void SetParent(Transform2D* parent) { m_pParent = parent; }

	void AddChild(Transform2D* child) { m_children.push_back(child); }

	void RemoveChild(Transform2D* child)
	{
		auto iter = std::find(m_children.begin(), m_children.end(), child);
		if (iter != m_children.end())
		{
			m_children.erase(iter);
		}
	}

	D2D1::Matrix3x2F GetLocalTM() const
	{
		return m_localTM;
	}

	D2D1::Matrix3x2F GetWorldTM() const
	{
		return m_worldTM;
	}


	void MakeTransformMatrix()
	{
		m_localTM = D2D1::Matrix3x2F::Scale(m_scale.x, m_scale.y, D2D1::Point2F(m_rotationCenter.x, m_rotationCenter.y)) *
					D2D1::Matrix3x2F::Rotation(m_rotation, D2D1::Point2F(m_rotationCenter.x, m_rotationCenter.y)) *
					D2D1::Matrix3x2F::Translation(m_position.x, m_position.y);

		if (m_pParent)
		{
			m_worldTM = m_localTM * m_pParent->GetWorldTM();
		}
		else
		{
			m_worldTM = m_localTM;
		}

		for (auto& child : m_children)
		{
			child->MakeTransformMatrix();
		}
	}

private:

	Transform2D* m_pParent = nullptr;
	std::vector<Transform2D*> m_children;

	D2D1::Matrix3x2F m_localTM;
	D2D1::Matrix3x2F m_worldTM;

	// Local
	mathHelper::Vector2F m_position = { 0.0f, 0.0f };
	mathHelper::Vector2F m_scale = { 1.0f, 1.0f };
	mathHelper::Vector2F m_rotationCenter = { 0.0f, 0.0f };

	float m_rotation = 0.0f;

public:

	static D2D1::Matrix3x2F RenderTM()	{ return _RenderTM;		}
	static D2D1::Matrix3x2F WorldTM()	{ return _WorldTM;		}
	static D2D1::Matrix3x2F CameraTM()	{ return _CameraTM;		}
	static D2D1::Matrix3x2F ScreenTM()  { return _ScreenTM;		}

	static void SetRenderTM(D2D1::Matrix3x2F tm)	{  _RenderTM	= tm; }
	static void SetWorldTM(D2D1::Matrix3x2F tm)		{  _WorldTM		= tm; }	
	static void SetScreenTM(D2D1::Matrix3x2F tm)	{  _ScreenTM	= tm; }


	static void SetCameraTM(D2D1::Matrix3x2F tm) { _CameraTM = tm; }

	static void SetCameraInvertTM(D2D1::Matrix3x2F tm) { D2D1InvertMatrix(&tm); _CameraTM = tm; }
	

private:

	static D2D1::Matrix3x2F _WorldTM;  // 월드 좌표계 변환을 위한 행렬
	static D2D1::Matrix3x2F _RenderTM; // 렌더링 변환을 위한 행렬을 일단, 만들어만 두었음
	static D2D1::Matrix3x2F _CameraTM; // 2D 카메라는 걍, 플레이어 따라다니는..? 스크린이랑 다른?
	static D2D1::Matrix3x2F _ScreenTM;
};