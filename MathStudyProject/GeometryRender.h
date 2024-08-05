/**
	@file      GeometryRender.h
	@brief     기하 도형을 랜더링 하는 클래스 입니다. D2D 초기화 및 래퍼로 별거 없어요..
	@author    SKYFISH
	@date      JUNE.2023
	@notice    52반 수업을 위한 테스트용 코드이며 수정 중에 있습니다.
**/

#pragma once

#include <windows.h>
#include <d2d1.h>
#include <d2d1_1helper.h>
#include <dwrite.h>
#include <vector>

class GeometryRender
{
	GeometryRender() = default;

public:
	GeometryRender(HWND hWin) { m_hWnd = hWin; }

	~GeometryRender();

	bool Initialize();

	bool BeginRender(D2D1::ColorF background = D2D1::ColorF::White);

	bool EndRender();

	void SetTransform(D2D1::Matrix3x2F tm);

	void DrawPoint(float x, float y, const D2D1::ColorF& color);

	void DrawLine(float x1, float y1, float x2, float y2, const D2D1::ColorF& color);

	void DrawLine(D2D1_POINT_2F start, D2D1_POINT_2F end, const D2D1::ColorF& color);

	void DrawCircle(float x, float y, float radius, const D2D1::ColorF& color);

	void DrawFillRectangle(float left, float top, float right, float bottom, const D2D1::ColorF& color);

	void DrawRectangle(float left, float top, float right, float bottom, const D2D1::ColorF& color);

	void DrawPolygon(std::vector< D2D1_POINT_2F>& pointList, const D2D1::ColorF& color);

	void DrawText(const wchar_t* text, float left, float top, float width, float height, const D2D1::ColorF& color);

	void DrawGrid(float x, float y, float width, float height, float interval, const D2D1::ColorF& color);

	const D2D1_SIZE_F Size() { return m_size;  }

private:

	void UnInitialize();

	bool InitD2D();
	void UninitD2D();
	
private:
	
	ID2D1Factory* m_pD2DFactory = nullptr;

	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;

	IDWriteFactory* m_pDWriteFactory = nullptr;

	IDWriteTextFormat* m_pDWriteTextFormat = nullptr;

	ID2D1SolidColorBrush* m_pBrush = nullptr;

	HWND m_hWnd;

	D2D1_SIZE_F m_size = { 100, 100 };
};

