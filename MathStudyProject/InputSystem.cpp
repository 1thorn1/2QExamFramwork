//***************************************************************************************
// InputSystem.cpp by SKYFISH (C) 2023, Just for Test
//***************************************************************************************

#include <windows.h>
#include "InputSystem.h"

InputSystem::~InputSystem()
{
}

void InputSystem::KeyDown(unsigned int key)
{
	isKeyDown[key] = true;
	isKey[key] = true;
}

void InputSystem::KeyUp(unsigned int key)
{
	isKeyUp[key] = true;
	isKey[key] = false;
}

bool InputSystem::IsKeyDown(unsigned int key)
{
	return isKeyDown[key];
}

bool InputSystem::IsKeyUp(unsigned int key)
{
	return isKeyUp[key];
}

bool InputSystem::IsKey(unsigned int key)
{
	return isKey[key];
}

bool InputSystem::MouseMove(int& prevX, int& prevY, int& curX, int& curY)
{
	prevX = m_prevMouse.x;
	prevY = m_prevMouse.y;

	curX = m_curMouse.x;
	curY = m_curMouse.y;

	if (m_curMouse.x == m_prevMouse.x && m_curMouse.y == m_prevMouse.y) return false;

	return true;
}

bool InputSystem::MouseLClicked(int& curX, int& curY)
{
	curX = m_curMouse.x;
	curY = m_curMouse.y;

	if (m_curMouse.left) return true;
	
	return false;
}

bool InputSystem::MouseLClickUp(int& curX, int& curY)
{
	if (m_prevMouse.left == true && m_curMouse.left == false)
	{
		curX = m_curMouse.x;
		curY = m_curMouse.y;
		return true;
	}
	return false;
}

bool InputSystem::MouseRClicked(int& curX, int& curY)
{
	if (m_curMouse.right)
	{
		curX = m_curMouse.x;
		curY = m_curMouse.y;

		return true;
	}

	return false;
}

bool InputSystem::MouseRClickUp(int& curX, int& curY)
{
	if (m_prevMouse.right == true && m_curMouse.right == false)
	{
		curX = m_curMouse.x;
		curY = m_curMouse.y;
		return true;
	}
	return false;
}

void InputSystem::ResetKey()
{
	for (int i = 0; i < 256; i++)
	{
		isKeyDown[i] = false;
		isKeyUp[i] = false;
	}
}

void InputSystem::UpdateMouse()
{
	if (m_hWind != ::GetActiveWindow()) return;
	POINT pt;
	if (FALSE == GetCursorPos(&pt)) return;
	if (FALSE == ScreenToClient(m_hWind, &pt)) return;

	if (m_screenRect.left > pt.x) return;
	if (m_screenRect.right < pt.x) return;
	if (m_screenRect.top > pt.y) return;
	if (m_screenRect.bottom < pt.y) return;

	m_prevMouse = m_curMouse;

	m_curMouse.x = pt.x;
	m_curMouse.y = pt.y;
	m_curMouse.wheel = 0;

	m_curMouse.left = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
	m_curMouse.right = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;
	m_curMouse.middle = (GetKeyState(VK_MBUTTON) & 0x8000) != 0;
}

void InputSystem::InitInput(HWND hWind, int mouseX, int mouseY)
{
	m_hWind = hWind;

	GetClientRect(m_hWind, &m_screenRect);

	for (int i = 0; i < 256; i++)
	{
		isKeyDown[i] = false;
		isKeyUp[i] = false;
		isKey[i] = false;
	}

}



