/**
	@file      BaseWindow.h
	@brief     윈도우 생성 및 메시지 처리를 담당하는 기반 클래스입니다.
	@author    SKYFISH
	@date      JUNE.2023
	@notice    52반 수업을 위한 테스트용 코드이며 수정 중에 있습니다.
**/

#pragma once

#include <Windows.h>

class BaseWindow
{
public:

	BaseWindow() = default;
	~BaseWindow() = default;

	void Initialize(HINSTANCE hInstance);
	
	virtual void Finalize() = 0;

	HINSTANCE GetInstance() const { return m_hInstance; }

	HWND GetWindow() const { return m_hWnd; }

	const int GetWidth() const { return m_width; }
	const int GetHeight() const { return m_height; }

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	HINSTANCE m_hInstance = { 0 };  // HINSTANCE is a handle to an instance of a module.
	HWND m_hWnd = { 0 };			// HWND is a handle to a window.

	int m_width = 1024;
	int m_height = 768;

	BaseWindow(const BaseWindow&) = delete;
	BaseWindow& operator=(const BaseWindow&) = delete;
};