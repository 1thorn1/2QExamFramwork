//***************************************************************************************
// Main.cpp by SKYFISH (C) 2023, Just for Test
//***************************************************************************************

#include "framework.h"
#include "GameManager.h"

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (false == GameManager::GetInstance()->Initialize(hInstance))
	{
		MessageBox(nullptr, L"GameManager Initialize Failed", L"Error", MB_OK);

		return EXIT_FAILURE;
	}

	GameManager::GetInstance()->Run();

	GameManager::GetInstance()->Finalize();

	return EXIT_SUCCESS;
}