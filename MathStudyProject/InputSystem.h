/**
	@file      InputSystem.h
	@brief     입력 관련 처리를 담당하는 클래스입니다. SingletonBase를 상속받아 구현되었습니다.
	@author    SKYFISH
	@date      JUNE.2023
	@notice    52반 수업을 위한 테스트용 코드이며 수정 중에 있습니다.
**/


#pragma once

#include "SingletonBase.h"

struct MouseState
{
	MouseState()
	{
		x = 0;
		y = 0;
		wheel = 0;
		left = false;
		right = false;
		middle = false;
	}

	int x;
	int y;
	int wheel;
	bool left;
	bool right;
	bool middle;
};

class InputSystem : public simpleSystem::SingletonBase<InputSystem>
{
	friend class simpleSystem::SingletonBase<InputSystem>;
	friend class GameManager;

	InputSystem() = default;
	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

	virtual ~InputSystem() override;

public:

	void KeyUp(unsigned int key);

	bool IsKeyDown(unsigned int key);

	bool IsKeyUp(unsigned int key);

	bool IsKey(unsigned int key);

	bool MouseMove(int& prevX,int& prevY, int& curX, int& curY);

	bool MouseLClicked(int& curX, int& curY);

	bool MouseLClickUp(int& curX, int& curY); // 마우스 L 버튼이 눌렸다가 떼어졌을 때

	bool MouseRClicked(int& curX, int& curY);

	bool MouseRClickUp(int& curX, int& curY); // 마우스 R 버튼이 눌렸다가 떼어졌을 때

private:
	void InitInput(HWND hWind, int mouseX, int mouseY);

	void UpdateScreenRect() { /* TODO!! */ }

	void KeyDown(unsigned int key);
	void UpdateMouse();

	void ResetKey();

	bool isKeyDown[256];
	bool isKeyUp[256];
	bool isKey[256];

	MouseState m_curMouse;
	MouseState m_prevMouse;

	HWND m_hWind;
	RECT m_screenRect;
};
