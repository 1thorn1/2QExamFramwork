//***************************************************************************************
// GameObject.cpp by SKYFISH (C) 2023, Just for Test
//***************************************************************************************


#include "GeometryObject.h"
#include "GeometryRender.h"
#include "DebugUtil.h"

using namespace mathHelper;

namespace testgeo
{
	void Circle::Render(GeometryRender* pRender)
	{
		
	}

	void Circle::Update()
	{
		
	}

	bool Circle::CheckInside(float x, float y)
	{
		return false;
	}


	void Rectangle::Render(GeometryRender* pRender)
	{
		D2D1::Matrix3x2F rectTM = m_transform.GetWorldTM();

		// ��ǥ�� ��ȯ : SetScreenTM �� Y-�� ���� ���ѵ� �� ���� ����. ȸ�� ������ �ݴ��.	

		D2D1::Matrix3x2F Transform = Transform2D::RenderTM() * rectTM * Transform2D::CameraTM() * Transform2D::ScreenTM();

		pRender->SetTransform(Transform);

		pRender->DrawRectangle(0, 0, m_size.width, m_size.height, m_color);


		// �������͸� ��ȯ �Ŀ�
		// DrawRectangle �� �׸� ���� ����.
		// �� ������ ���� ��ȯ�ؼ� �������� �׷�����.
		// RenderTM �� ScreenTM �� ���ؼ� ��ǥ�踦 ��ȯ�� ���� �׷��� ���� ��츦 ���غ���.
		pRender->SetTransform(Transform2D::WorldTM());
		//pRender->SetTransform(Transform2D::RenderTM());
		//pRender->SetTransform(Transform2D::ScreenTM());
		//pRender->SetTransform(Transform2D::RenderTM() * Transform2D::ScreenTM());

		D2D1_POINT_2F TL = rectTM.TransformPoint(D2D1::Point2F(0, 0));
		D2D1_POINT_2F BL = rectTM.TransformPoint(D2D1::Point2F(0, m_size.height));
		D2D1_POINT_2F TR = rectTM.TransformPoint(D2D1::Point2F(m_size.width, 0));
		D2D1_POINT_2F BR = rectTM.TransformPoint(D2D1::Point2F(m_size.width, m_size.height));

		pRender->DrawLine(TL, BL, D2D1::ColorF::Red);
		pRender->DrawLine(BL, BR, D2D1::ColorF::Blue); // �� ���� �׷����� ��ġ�� ����, Y-�� ������ �Ǿ��ִ� ���� �� �� �ִ�.
		pRender->DrawLine(BR, TR, D2D1::ColorF::Purple);
		pRender->DrawLine(TR, TL, D2D1::ColorF::Green);
	}

	void Rectangle::Update()
	{
		
	}

	bool Rectangle::CheckInside(float x, float y)
	{
		return false;
	}

	void Polygon::Render(GeometryRender* pRender)
	{
		// ��ũ������ �Է� ���� ��ǥ�� �׸��� ������ ���� ���̴� ��ǥ�踦 �����ؾ� �Ѵ�.
		pRender->SetTransform(Transform2D::WorldTM());

		//pRender->SetTransform(Transform2D::RenderTM() * Transform2D::ScreenTM());
	
		std::vector<D2D1_POINT_2F> render_pointList;

		D2D1::Matrix3x2F polygonTM = m_transform.GetWorldTM();

		int nOrder = 0;
		for (auto& point : m_pointList)
		{	
			render_pointList.emplace_back(polygonTM.TransformPoint(point));

			if (IsSelected())
			{
				pRender->DrawCircle(point.x, point.y, 5, D2D1::ColorF::Red);
				pRender->DrawText(std::to_wstring(nOrder).c_str(), point.x, point.y, 3, 3, D2D1::ColorF::Blue);
			}

			nOrder++;
		}

		pRender->DrawPolygon(render_pointList, m_color);

		render_pointList.clear();

	}

	void Polygon::Update()
	{
		
	}

	bool Polygon::CheckInside(float x, float y)
	{
		std::vector <D2D1_POINT_2F> pointList = m_pointList;

		pointList.push_back(m_pointList.front());

		//if (0 == cn_PnPoly(D2D1::Point2F(x, y), pointList, pointList.size()-1)) // crossing number test
		if (0 == wn_PnPoly(D2D1::Point2F(x, y), pointList, pointList.size() - 1)) // winding number test
		{
			return false;
		}
		return true;
	}

}
