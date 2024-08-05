#include "stdafx.h"
#include "Game.h"

wchar_t g_szTitle[128] = L"ShootingGame";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;	
	wc.hInstance = hInstance;   
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"BasicWindowClass";

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"윈도우 클래스 등록 실패", L"에러", MB_OK | MB_ICONERROR);
		return 1;
	}

	SIZE clientSize = { 1024, 768 };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowEx(0, L"BasicWindowClass", g_szTitle, WS_OVERLAPPEDWINDOW,0,0,
		clientRect.right-clientRect.left,  clientRect.bottom - clientRect.top , NULL, NULL, hInstance, NULL );

	if (!hwnd)
	{
		MessageBox(NULL, L"윈도우 생성 실패", L"에러", MB_OK | MB_ICONERROR);
		return 1;
	}
		
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	Game::Initialize(hwnd, clientSize.cx, clientSize.cy);

	MSG msg;
	while (true)
	{		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{					
			Game::Update();
			Game::Render();
		}
	}
	Game::Uninitialize();
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
