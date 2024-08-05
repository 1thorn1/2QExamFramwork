/**
	@file      InputSystem.h
	@brief     �Է� ���� ó���� ����ϴ� Ŭ�����Դϴ�. SingletonBase�� ��ӹ޾� �����Ǿ����ϴ�.
	@author    SKYFISH
	@date      JUNE.2023
	@notice    52�� ������ ���� �׽�Ʈ�� �ڵ��̸� ���� �߿� �ֽ��ϴ�.
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

	bool MouseLClickUp(int& curX, int& curY); // ���콺 L ��ư�� ���ȴٰ� �������� ��

	bool MouseRClicked(int& curX, int& curY);

	bool MouseRClickUp(int& curX, int& curY); // ���콺 R ��ư�� ���ȴٰ� �������� ��

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
