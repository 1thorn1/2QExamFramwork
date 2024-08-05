/**
	@file      GeometryObject.h
	@brief     테스트를 위한 기하 도형들 입니다. 여기를 주로 수정하고 테스트 하게 되겠네요.
	@author    SKYFISH
	@date      JUNE.2023
	@notice    52반 수업을 위한 테스트용 코드이며 수정 중에 있습니다.
**/


#pragma once

#include "transform2D.h"
#include <string>

class GeometryRender;

class GeometryObject
{
public:
	GeometryObject() = default;
	virtual ~GeometryObject() = default;

	virtual void Render(GeometryRender* pRender) = 0;
	virtual void Update() = 0;

	virtual bool CheckInside(float x, float y) = 0;

	void Translation(float x, float y)
	{
		m_transform.SetPosition(x, y);

		m_transform.MakeTransformMatrix();
	}

	void Rotate(float angle)
	{
		m_transform.SetRotation(angle);

		m_transform.SetRotationCenter(m_rotateCenter.x, m_rotateCenter.y);

		m_transform.MakeTransformMatrix();
	}

	void Scale(float x, float y)
	{
		m_transform.SetScale(x, y);

		m_transform.MakeTransformMatrix();
	}

	void SetRotateCenter(float x, float y)
	{
		m_rotateCenter.x = x;
		m_rotateCenter.y = y;
	}

	void SetColor(const D2D1::ColorF& color)
	{
		m_color = color;
	}

	const D2D1::ColorF& GetColor() const 
	{ 
		return m_color; 
	}

	void AddChild(GeometryObject* pChild)
	{
		m_transform.AddChild(&pChild->GetTransform());
	}

	void RemoveChild(GeometryObject* pChild)
	{
		m_transform.RemoveChild(&pChild->GetTransform());
	}

	void SetParent(GeometryObject* pParent)
	{
		m_transform.SetParent(&pParent->GetTransform());

		m_transform.MakeTransformMatrix();
	}

	Transform2D& GetTransform() 
	{ 
		return m_transform; 
	}

	void SetName(const std::string_view& name) 
	{ 
		m_name = name; 
	}

	const char* GetName() const 
	{ 
		return m_name.c_str();
	}

	void SetSelected(bool bOnOff) { m_bSelected = bOnOff; }

	bool IsSelected() { return m_bSelected;  }

protected:
	
	Transform2D m_transform;

	mathHelper::Vector2F m_rotateCenter = { 0, 0 };

	D2D1::ColorF m_color = D2D1::ColorF::Black;

	std::string m_name = "default";

	bool m_bSelected = false;
};

namespace testgeo
{
	class Circle : public GeometryObject
	{
	public:
		Circle() = default;
		virtual ~Circle() = default;

		void Initialize(float x, float y, float radius)
		{
			m_radius = radius;

			Translation(x, y);
		}

		virtual void Render(GeometryRender* pRender) override;
		virtual void Update() override;
		virtual bool CheckInside(float x, float y) override;

	private:

		float m_radius;
	};


	class Rectangle : public GeometryObject
	{
	public:
		Rectangle() = default;
		virtual ~Rectangle() = default;

		void Initialize(float x, float y, float width, float height)
		{
			m_size.width = width;
			m_size.height = height;

			Translation(x, y);
		}

		virtual void Render(GeometryRender* pRender) override;
		virtual void Update() override;
		virtual bool CheckInside(float x, float y) override;

	private:
		
		D2D1_SIZE_F m_size = { 0.f, 0.f };
	};

	class Polygon : public GeometryObject
	{
	public:
		Polygon() = default;
		virtual ~Polygon() = default;

		void Initialize(float x, float y, std::vector< D2D1_POINT_2F>& pointList)
		{
			// int Func(int x, int y) 와 int Func(int a) 호출할 때, 파라메터에 따라 다른 함수가 호출됨
			// 재정의한 오퍼레이터도 오버로딩이 됩니다. 파라메터가 L-Value 인가 R-Value 인가에 따라서.
			//td::move 는 L-Value 를 우측값이 R-Value 캐스팅 해주는 함수.
			m_pointList = std::move(pointList); // 이동에 관한 operator= (T&&) 이게 호출됨
			//m_pointList = pointList; // 복사 operator= (T&)
		
			m_rotateCenter.x = m_pointList.back().x;
			m_rotateCenter.y = m_pointList.back().y;
			
			Translation(x, y);
		}

		virtual void Render(GeometryRender* pRender) override;
		virtual void Update() override;
		virtual bool CheckInside(float x, float y) override;

	private:

		std::vector<D2D1_POINT_2F> m_pointList;
	};
}



