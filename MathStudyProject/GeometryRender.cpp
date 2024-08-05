//***************************************************************************************
// GeometryRender.cpp by SKYFISH (C) 2023, Just for Test
//***************************************************************************************


#include "GeometryRender.h"
#include <stdio.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib,"dwrite.lib")

using namespace D2D1;

#define SAFE_RELEASE(P) if(P){P->Release() ; P = nullptr;}

bool INVALID_RESULT(HRESULT hr)
{
	if (S_OK != hr)
	{
		printf("GeometryRender Error : %d\n", hr);

		return true;

	}

	return false;
}

GeometryRender::~GeometryRender()
{
	UnInitialize();
}

bool GeometryRender::Initialize()
{
	InitD2D();

	return false;
}

bool GeometryRender::BeginRender(D2D1::ColorF background)
{
	if(nullptr == m_pRenderTarget) return false;

	m_pRenderTarget->BeginDraw();

	m_pRenderTarget->Clear(background);

	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	return true;
}

bool GeometryRender::EndRender()
{
	if (INVALID_RESULT( m_pRenderTarget->EndDraw() )) return false;

	return true;
}

void GeometryRender::SetTransform(D2D1::Matrix3x2F tm)
{
	if (m_pRenderTarget) m_pRenderTarget->SetTransform(tm);
}

void GeometryRender::DrawPoint(float x, float y, const D2D1::ColorF& color)
{
	m_pBrush->SetColor(color);
	m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), 2, 2), m_pBrush);
}

void GeometryRender::DrawLine(float x1, float y1, float x2, float y2, const D2D1::ColorF& color)
{
	m_pBrush->SetColor(color);
	m_pRenderTarget->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), m_pBrush);
}

void GeometryRender::DrawLine(D2D1_POINT_2F start, D2D1_POINT_2F end, const D2D1::ColorF& color)
{
	m_pBrush->SetColor(color);
	m_pRenderTarget->DrawLine(start, end, m_pBrush);
}

void GeometryRender::DrawCircle(float x, float y, float radius, const D2D1::ColorF& color)
{
	m_pBrush->SetColor(color);
	m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius) , m_pBrush);
}

void GeometryRender::DrawFillRectangle(float left, float top, float right, float bottom, const D2D1::ColorF& color)
{
	m_pBrush->SetColor(color);
	m_pRenderTarget->FillRectangle(D2D1::Rect(left, top, right, bottom), m_pBrush);
}

void GeometryRender::DrawRectangle(float left, float top, float right, float bottom, const D2D1::ColorF& color)
{
	m_pBrush->SetColor(color);
	m_pRenderTarget->DrawRectangle(D2D1::Rect(left, top, right, bottom), m_pBrush);
}

void GeometryRender::DrawPolygon(std::vector<D2D1_POINT_2F>& pointList, const D2D1::ColorF& color)
{
	if (pointList.size() < 3) return;

	m_pBrush->SetColor(color);

	auto startIt = pointList.begin();
	auto endIt = startIt;
	
	for (auto it = startIt; it != pointList.end(); it++)
	{
		auto nextIt = it + 1;
		if (nextIt == pointList.end()) break;
		
		m_pRenderTarget->DrawLine(*it, *nextIt, m_pBrush);

		endIt = nextIt;
	}

	if (startIt != endIt)
	{
		m_pRenderTarget->DrawLine(*startIt, *endIt, m_pBrush);
	}
}

void GeometryRender::DrawText(const wchar_t* text, float left, float top, float width, float height, const D2D1::ColorF& color)
{
	m_pBrush->SetColor(color);

	m_pRenderTarget->DrawTextW(text, wcslen(text), m_pDWriteTextFormat, D2D1::Rect(left, top, left + width, top + height), m_pBrush);
}

void GeometryRender::DrawGrid(float x, float y, float width, float height, float interval, const D2D1::ColorF& color)
{
	for (float i = x; i < x + width; i += interval)
	{
		DrawLine(i, y, i, y + height, color);
	}

	for (float i = y; i < y + height; i += interval)
	{
		DrawLine(x, i, x + width, i, color);
	}
}

void GeometryRender::UnInitialize()
{
	UninitD2D();
}

bool GeometryRender::InitD2D()
{
	if (INVALID_RESULT( CoInitialize(NULL)) ) return false;

	if (INVALID_RESULT( D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory))) return false;

	RECT rc;
	GetClientRect(m_hWnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top);

	// Direct2D 렌터 타겟을 만듭니다.
	if (INVALID_RESULT( m_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_hWnd, size),
		&m_pRenderTarget) )) return false;


	// DirectWrite 팩터리를 만듭니다.
	if (INVALID_RESULT( DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pDWriteFactory)) )) return false;
		

	if (INVALID_RESULT( m_pDWriteFactory->CreateTextFormat(
		L"", // FontName    제어판-모든제어판-항목-글꼴-클릭 으로 글꼴이름 확인가능
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		15.0f,   // Font Size
		L"", //locale
		&m_pDWriteTextFormat ) )) return false;


	// 텍스트 정렬 방식 설정
	m_pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		
		
	if (INVALID_RESULT( m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black), &m_pBrush) )) return false;


	m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

	if (m_pRenderTarget)
	{
		m_size = m_pRenderTarget->GetSize(); // 렌더 타겟을 재설정하지 않는 이상, 항상 동일
	}

	return true;
}

void GeometryRender::UninitD2D()
{
	SAFE_RELEASE(m_pD2DFactory);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pDWriteFactory);
	SAFE_RELEASE(m_pDWriteTextFormat);
	SAFE_RELEASE(m_pBrush);

	CoUninitialize();
}


