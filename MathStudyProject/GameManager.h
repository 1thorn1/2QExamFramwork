/**
	@file      GameManager.h
	@brief     Template C++ Singleton Base class
	@author    SKYFISH
	@date      JUNE.2023
	@notice    52반 수업을 위한 테스트용 코드이며 수정 중에 있습니다.
**/

#pragma once

#include "SingletonBase.h"
#include "MainWindow.h"
#include "TimeSystem.h"
#include "SimpleMathHelper.h"

#include <vector>

class GeometryRender;
class GeometryObject;

class GameManager : public simpleSystem::SingletonBase<GameManager>
{
	friend class simpleSystem::SingletonBase<GameManager>;

	GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	virtual ~GameManager() override;

public:

	bool Initialize(HINSTANCE hInstance);

	void Run();

	void Finalize();

private:

	void Prepare();

	void Update();
	
	void Render();

	void CleanUp();

#pragma region _input_proc

	void KeyDownProc();

	void MouseMove();

	void MouseLButton();
	
	void MouseRButton();

#pragma endregion

#pragma region _geometry

	void CreateGeometryObject();

	void CreateConvexPolygon();

	void CreatePolygon();

	void DeleteGeometryObject() { /*TODO*/ };

	void SelectGeometryObject(int x, int y);

	void RotateAllObject(float rotation);

	void RotateSelectedObject(float rotation);

#pragma endregion

#pragma region _debug

	void DebugPrint();

	void PrintMatrix();

#pragma endregion


#pragma region _member_data
	GeometryRender* m_pRender = nullptr;

	mathHelper::Vector2F m_cameraPos;

	GameTimer m_Timer;

	MainWindow m_mainWindow;

	std::vector< GeometryObject*> m_objects;

	std::vector<D2D1_POINT_2F> m_points;

	bool m_bToggleGrid = false;
#pragma endregion
};

