/**
	@file      GeometryObject.h
	@brief     �׽�Ʈ�� ���� ���� ������ �Դϴ�. ���⸦ �ַ� �����ϰ� �׽�Ʈ �ϰ� �ǰڳ׿�.
	@author    SKYFISH
	@date      JUNE.2023
	@notice    52�� ������ ���� �׽�Ʈ�� �ڵ��̸� ���� �߿� �ֽ��ϴ�.
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
			// int Func(int x, int y) �� int Func(int a) ȣ���� ��, �Ķ���Ϳ� ���� �ٸ� �Լ��� ȣ���
			// �������� ���۷����͵� �����ε��� �˴ϴ�. �Ķ���Ͱ� L-Value �ΰ� R-Value �ΰ��� ����.
			//td::move �� L-Value �� �������� R-Value ĳ���� ���ִ� �Լ�.
			m_pointList = std::move(pointList); // �̵��� ���� operator= (T&&) �̰� ȣ���
			//m_pointList = pointList; // ���� operator= (T&)
		
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



