
//***************************************************************************************
// GameManager.cpp by SKYFISH (C) 2023, Just for Test
//***************************************************************************************


#include "GameManager.h"

#include "GeometryRender.h"
#include "GeometryObject.h"

#include "InputSystem.h"
#include "MainWindow.h"

#include "DebugUtil.h"

#include <random>
#include <string>
#include <algorithm>
#include <stack>
#include <cassert>

namespace global
{
	const D2D1::ColorF Colors[7] =
	{
		D2D1::ColorF::Red, D2D1::ColorF::Orange, D2D1::ColorF::Yellow,
		D2D1::ColorF::Green, D2D1::ColorF::Blue, D2D1::ColorF::Indigo, D2D1::ColorF::Purple
	};

	const std::string ColorString[8] =
	{
		"Red", "Orange", "Yellow", "Green", "Blue", "Indigo", "Purple"
	};
}


GameManager::~GameManager()
{
}

bool GameManager::Initialize(HINSTANCE hInstance)
{
	m_mainWindow.Initialize(hInstance);
    
	m_pRender = new GeometryRender(m_mainWindow.GetWindow());

	m_pRender->Initialize();

	InputSystem::GetInstance()->InitInput(m_mainWindow.GetWindow(), m_mainWindow.GetWidth() / 2, m_mainWindow.GetHeight() / 2);
	
	return true;
}

void GameManager::Run()
{
	MSG msg;

	Prepare();

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			if (msg.message == WM_KEYDOWN)
			{
				InputSystem::GetInstance()->KeyDown(msg.wParam);
			}
			else if (msg.message == WM_KEYUP)
			{
				InputSystem::GetInstance()->KeyUp(msg.wParam);
			}

			else
			{
				DispatchMessage(&msg);
			}
		}
		else
		{
			m_Timer.Tick();

			Update();

			Render();

		}
	}

	CleanUp();

}

void GameManager::Finalize()
{
	if (m_pRender)
	{
		delete m_pRender;
		m_pRender = nullptr;
	}

	m_mainWindow.Finalize();
}

void GameManager::Prepare()
{
	const D2D1_SIZE_F size = m_pRender->Size();

	// screen 좌표계에서 y축 반전 하므로  이전에 반전하게한다.
	Transform2D::SetRenderTM(D2D1::Matrix3x2F::Scale(1.0f, -1.0f));


	// 왼쪽 하단이 원점이 되게하는 변환
	Transform2D::SetScreenTM(D2D1::Matrix3x2F::Scale(1.0f, -1.0f) * D2D1::Matrix3x2F::Translation(0.0f, (float)size.height));

	// 카메라의 월드 위치로 역행렬을 계산하여 카메라 좌표계로 변환하는 행렬
	Transform2D::SetCameraInvertTM(D2D1::Matrix3x2F::Translation(m_cameraPos.x, m_cameraPos.y));

	// 기준	박스
	testgeo::Rectangle* pRect = new testgeo::Rectangle;

	pRect->Initialize(size.width * 0.5, size.height * 0.5 , 100, 100);

	pRect->SetColor(D2D1::ColorF::Black);

	pRect->SetName("First Rectangle");
	
	m_objects.emplace_back(pRect);

	// 시간
	m_Timer.Reset();
}

void GameManager::CleanUp()
{
	for (auto obj : m_objects)
	{
		delete obj;
	}

	m_objects.clear();
}

void GameManager::KeyDownProc()
{
	//https://docs.microsoft.com/ko-kr/windows/win32/inputdev/virtual-key-codes
	//
	// '1' : 라인, '2' : 원, '3' : 다각형, '4' : 볼록 다각형
	// 'Q' : 선택된 것만 오른쪽 회전
	// 'R' : 선택된 것만 왼쪽 회전
	// 'D' : 디버그 정보 출력
	// VK_LEFT/VK_RIGHT : 좌우 카메라 이동
	// TAB : 토글 그리드

	if (InputSystem::GetInstance()->IsKeyDown(VK_LEFT))
	{
		m_cameraPos.x -= 10.0f;

		Transform2D::SetCameraInvertTM(D2D1::Matrix3x2F::Translation(m_cameraPos.x, m_cameraPos.y));
	}
	else if (InputSystem::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		m_cameraPos.x += 10.0f;

		Transform2D::SetCameraInvertTM(D2D1::Matrix3x2F::Translation(m_cameraPos.x, m_cameraPos.y));
	}
	else if (InputSystem::GetInstance()->IsKeyDown(VK_TAB))
	{
		m_bToggleGrid = !m_bToggleGrid;
	}
	else if (InputSystem::GetInstance()->IsKeyDown('1'))
	{
		//todo
	}
	else if (InputSystem::GetInstance()->IsKeyDown('2'))
	{
		//todo
	}
	else if (InputSystem::GetInstance()->IsKeyDown('3'))
	{
		CreatePolygon();
	}
	else if (InputSystem::GetInstance()->IsKeyDown('4'))
	{
		CreateConvexPolygon();
	}
	else if (InputSystem::GetInstance()->IsKeyDown('Q'))
	{
		RotateSelectedObject(1.0f);
	}
	else if (InputSystem::GetInstance()->IsKeyDown('W'))
	{
		RotateSelectedObject(-1.0f);
	}
	else if (InputSystem::GetInstance()->IsKeyDown('D'))
	{
		DebugPrint();
	}

}

void GameManager::MouseMove()
{
	static int prevX; static int prevY;
	static int curX;  static int curY;

	if (false == InputSystem::GetInstance()->MouseMove(prevX, prevY, curX, curY)) return;

	// TODO: 마우스 이동 처리
}

void GameManager::MouseLButton()
{
	static int x; static int y;

	if (false == InputSystem::GetInstance()->MouseLClickUp(x, y)) return;

	bool bExisted = false;
	for (const auto point : m_points)
	{
		if (point.x == x && point.y == y)
		{
			bExisted = true;
			break;
		}
	}

	if (false == bExisted)
	{
		m_points.push_back(D2D1::Point2F(x, y));

		printf("MouseLButton : x : %d, y : %d\n", x, y);
	}
}

void GameManager::MouseRButton()
{
	static int x; static int y;

	if (false == InputSystem::GetInstance()->MouseRClickUp(x, y)) return;

	// 마우스 R 버튼 처리
	printf("MouseRButton %d, %d\n", x, y);
	SelectGeometryObject(x, y);
}

void GameManager::CreateGeometryObject()
{
	static int colorIndex = -1;

	if (m_points.size() < 3) return;

	testgeo::Polygon * pPolygon = new testgeo::Polygon;

	// 벡터가 복사되지 않도록 참조로 넘김
	pPolygon->Initialize(0, 0, m_points);

	colorIndex = (++colorIndex) % 7;

	pPolygon->SetColor(global::Colors[colorIndex]);

	pPolygon->SetName(global::ColorString[colorIndex] + " Polygon");

	if (m_objects.empty() == false)
	{
		// 새로 생성된 폴리곤-기존 마지막 오브젝트 -> 부모-자식 설정
		pPolygon->AddChild(m_objects.back());
		m_objects.back()->SetParent(pPolygon);
	}

	m_objects.push_back(pPolygon);

	m_points.clear();
}

void GameManager::CreateConvexPolygon()
{
	if (m_points.size() < 3) return;	

	std::vector<D2D1_POINT_2F> checkPoints(std::move(m_points));

	auto CompareYX = [](const D2D1_POINT_2F& a, const D2D1_POINT_2F& b)->bool {

		return (a.y == b.y) ? (a.x == b.x) : (a.y > b.y);
	};

	//  y, x 좌표 내림차순 순으로 정렬 ( 스크린 좌표계라 헷갈려 해서. )
	std::sort(checkPoints.begin(), checkPoints.end(), CompareYX);

	D2D1_POINT_2F basePoint = checkPoints.front();

	auto ComapreCCW = [=](const D2D1_POINT_2F& a, const D2D1_POINT_2F& b) {

		int c = mathHelper::IsLeft(basePoint, a, b);

		// 직선상에 있다면 가까운 점 먼저!
		if (c == 0)
		{
			int distA = (basePoint.x - a.x) * (basePoint.x - a.x) + (basePoint.y - a.y) * (basePoint.y - a.y);
			int distB = (basePoint.x - b.x) * (basePoint.x - b.x) + (basePoint.y - b.y) * (basePoint.y - b.y);

			return (distA < distB);
		}

		return (c > 0);
	};

	// 첫번째 점을 기준으로 반시계 방향으로 정렬
	std::sort(checkPoints.begin(), checkPoints.end(), ComapreCCW);

	// 첫번째 점을 기준으로 반시계 방향으로 정렬된 점들을 순회하며
	// 기준점, 순회점, 순회점 다음점이 좌회전이면 다음점으로 넘어가고
	// 우회전이면 순회점을 제거한다.
	std::stack<int> idxStack;

	idxStack.push(0); // First
	idxStack.push(1); // Second

	int next = 2;	  // third Point index

	int n = checkPoints.size();

	while (next < n)
	{
		while (idxStack.size() >= 2) // 비교를 위해 점 2개가 필요해요.
		{
			int second = idxStack.top(); idxStack.pop();
			int first = idxStack.top();

			int c = mathHelper::IsLeft(checkPoints[first], checkPoints[second], checkPoints[next]);

			if (c > 0)
			{
				idxStack.push(second); // first, second, next 가 좌회전이라면 second 를 스택에 다시 푸시			
				break;
			}
		}//while

		idxStack.push(next++);
	}//while

	// 스텍에 남은 포인트를 가지고 볼록 다각형을 만들 수 있습니다.

	m_points.clear();

	while (idxStack.size() > 0)
	{
		m_points.push_back(checkPoints[idxStack.top()]);

		idxStack.pop();
	}

	CreateGeometryObject();

}

void GameManager::CreatePolygon()
{
	if (m_points.size() < 3) return;

	std::vector<D2D1_POINT_2F> checkPoints(std::move(m_points));

	auto CompareYX = [](const D2D1_POINT_2F& a, const D2D1_POINT_2F& b)->bool {

		return (a.y == b.y) ? (a.x == b.x) : (a.y > b.y);
	};

	// 스크린 좌표계라 헷갈려 하길래  y, x 좌표 내림차순 정렬
	std::sort(checkPoints.begin(), checkPoints.end(), CompareYX);

	D2D1_POINT_2F basePoint = checkPoints.front();

	auto ComapreCCW = [=](const D2D1_POINT_2F& a, const D2D1_POINT_2F& b) {

		int c = mathHelper::IsLeft(basePoint, a, b);

		// 직선상에 있다면 가까운 점 먼저!
		if (c == 0)
		{
			int distA = (basePoint.x - a.x) * (basePoint.x - a.x) + (basePoint.y - a.y) * (basePoint.y - a.y);
			int distB = (basePoint.x - b.x) * (basePoint.x - b.x) + (basePoint.y - b.y) * (basePoint.y - b.y);

			return (distA < distB);
		}
		
		return (c < 0); // 위아래가 바뀌니 반시계 방향으로 볼래면 이것도 바꿔줘야 해요.
		//return (c > 0);
	};

	// 첫번째 점을 기준으로 반시계 방향으로 정렬
	std::sort(checkPoints.begin(), checkPoints.end(), ComapreCCW);

	//정렬만 하고 만들어 보고 비교해 봅니다.

	m_points = std::move(checkPoints);

	CreateGeometryObject();
}

void GameManager::SelectGeometryObject(int x, int y)
{
	for (auto obj : m_objects)
	{
		if (obj->CheckInside(x, y))
		{
			obj->SetSelected( !(obj->IsSelected()));
			break;
		}
	}
}

void GameManager::RotateAllObject(float rotation)
{
	// 테스트 목적으로 회전 누적
	static float _rot = 0.f;
	_rot += rotation;

	for (auto obj : m_objects)
	{
		obj->Rotate(_rot);
	}
}

void GameManager::RotateSelectedObject(float rotation)
{
	// 테스트 목적으로 회전 누적
	static float _rot = 0.f;
	_rot += rotation;

	for (auto obj : m_objects)
	{
		if (obj->IsSelected())
			obj->Rotate(_rot);
	}
}


void GameManager::DebugPrint()
{
	mathHelper::Vector2F position;

	for (auto obj : m_objects)
	{
		const Transform2D& transform = obj->GetTransform();

		DEBUGPRINT("\n \n Name: %s \n", obj->GetName());
			
		DEBUGPRINT("localTM:\n { %.1f, %.1f }\n { %.1f, %.1f }\n { %.1f, %.1f } \n ",
			transform.GetLocalTM()._11, transform.GetLocalTM()._12,
			transform.GetLocalTM()._21, transform.GetLocalTM()._22,
			transform.GetLocalTM()._31, transform.GetLocalTM()._32);

		DEBUGPRINT("worldTM:\n { %.1f, %.1f }\n { %.1f, %.1f }\n { %.1f, %.1f } \n ",
			transform.GetWorldTM()._11, transform.GetWorldTM()._12,
			transform.GetWorldTM()._21, transform.GetWorldTM()._22,
			transform.GetWorldTM()._31, transform.GetWorldTM()._32);


		D2D1::Matrix3x2F Transform = Transform2D::RenderTM() * transform.GetWorldTM() * Transform2D::CameraTM() * Transform2D::ScreenTM();

		DEBUGPRINT("Transform:\n { %.1f, %.1f }\n { %.1f, %.1f }\n { %.1f, %.1f } \n ",
			Transform._11, Transform._12,
			Transform._21, Transform._22,
			Transform._31, Transform._32);
	}
}


void GameManager::PrintMatrix()
{
	m_pRender->SetTransform(D2D1::Matrix3x2F::Identity());

	m_pRender->DrawFillRectangle(0, 0, 200, 200, D2D1::ColorF::Beige);

	// 효율을 버린 디버깅과 학습을 위한 코드
	D2D1::Matrix3x2F renderTM(Transform2D::RenderTM());
	D2D1::Matrix3x2F worldTM(Transform2D::WorldTM());
	D2D1::Matrix3x2F cameraTM(Transform2D::CameraTM());
	D2D1::Matrix3x2F screenTM(Transform2D::ScreenTM());

	WCHAR buffer[256] = { 0 };
	swprintf_s(buffer, L" localTM:\n { %.1f, %.1f }\n { %.1f, %.1f }\n { %.1f, %.1f } \n ",
		renderTM._11, renderTM._12, renderTM._21, renderTM._22, renderTM._31, renderTM._32);

	m_pRender->DrawText(buffer, 5, 5, 100, 100, D2D1::ColorF::Black);

	swprintf_s(buffer, L" worldTM:\n { %.1f, %.1f }\n { %.1f, %.1f }\n { %.1f, %.1f } \n ",
		worldTM._11, worldTM._12, worldTM._21, worldTM._22, worldTM._31, worldTM._32);

	m_pRender->DrawText(buffer, 105, 5, 100, 100, D2D1::ColorF::Black);

	swprintf_s(buffer, L" cameraTM:\n { %.1f, %.1f }\n { %.1f, %.1f }\n { %.1f, %.1f } \n ",
		cameraTM._11, cameraTM._12, cameraTM._21, cameraTM._22, cameraTM._31, cameraTM._32);

	m_pRender->DrawText(buffer, 5, 105, 100, 100, D2D1::ColorF::Black);

	swprintf_s(buffer, L" screenTM:\n { %.1f, %.1f }\n { %.1f, %.1f }\n { %.1f, %.1f } \n ",
		screenTM._11, screenTM._12, screenTM._21, screenTM._22, screenTM._31, screenTM._32);

	m_pRender->DrawText(buffer, 105, 105, 100, 100, D2D1::ColorF::Black);

}


void GameManager::Update()
{
	InputSystem::GetInstance()->UpdateMouse();

	MouseMove();
	MouseLButton();
	MouseRButton();
	KeyDownProc();

	for (auto obj : m_objects)
	{
		obj->Update();
	}

	InputSystem::GetInstance()->ResetKey();
}

void GameManager::Render()
{
	m_pRender->BeginRender();

	if (m_bToggleGrid)
	{
		m_pRender->DrawGrid(0, 0, m_mainWindow.GetWidth(), m_mainWindow.GetHeight(), 20, D2D1::ColorF::LightGray);
	}

	for (auto point : m_points)
	{
		m_pRender->DrawPoint(point.x, point.y, D2D1::ColorF::Red);
	}

	for (auto obj : m_objects)
	{
		obj->Render(m_pRender);
	}

	PrintMatrix();

	m_pRender->EndRender();
}