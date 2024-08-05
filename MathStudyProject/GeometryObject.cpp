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

		// 좌표계 변환 : SetScreenTM 이 Y-축 반전 시켜둔 걸 잊지 말자. 회전 방향이 반대다.	

		D2D1::Matrix3x2F Transform = Transform2D::RenderTM() * rectTM * Transform2D::CameraTM() * Transform2D::ScreenTM();

		pRender->SetTransform(Transform);

		pRender->DrawRectangle(0, 0, m_size.width, m_size.height, m_color);


		// 지오메터리 변환 후에
		// DrawRectangle 로 그릴 수가 없다.
		// 네 정점을 각각 변환해서 라인으로 그려본다.
		// RenderTM 과 ScreenTM 을 곱해서 좌표계를 변환한 경우와 그렇지 않은 경우를 비교해본다.
		pRender->SetTransform(Transform2D::WorldTM());
		//pRender->SetTransform(Transform2D::RenderTM());
		//pRender->SetTransform(Transform2D::ScreenTM());
		//pRender->SetTransform(Transform2D::RenderTM() * Transform2D::ScreenTM());

		D2D1_POINT_2F TL = rectTM.TransformPoint(D2D1::Point2F(0, 0));
		D2D1_POINT_2F BL = rectTM.TransformPoint(D2D1::Point2F(0, m_size.height));
		D2D1_POINT_2F TR = rectTM.TransformPoint(D2D1::Point2F(m_size.width, 0));
		D2D1_POINT_2F BR = rectTM.TransformPoint(D2D1::Point2F(m_size.width, m_size.height));

		pRender->DrawLine(TL, BL, D2D1::ColorF::Red);
		pRender->DrawLine(BL, BR, D2D1::ColorF::Blue); // 이 선이 그려지는 위치를 보면, Y-축 반전이 되어있는 것을 알 수 있다.
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
		// 스크린에서 입력 받은 좌표로 그리기 때문에 눈에 보이는 좌표계를 설정해야 한다.
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
