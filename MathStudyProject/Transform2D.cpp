//***************************************************************************************
// Transform2D.cpp by SKYFISH (C) 2023, Just for Test
//***************************************************************************************


#include "Transform2D.h"


D2D1::Matrix3x2F Transform2D::_WorldTM = D2D1::Matrix3x2F::Identity(); // ���� ��ǥ�� ��ȯ�� ���� ���

D2D1::Matrix3x2F Transform2D::_RenderTM = D2D1::Matrix3x2F::Identity(); // ������ ��ȯ�� ���� ���   

D2D1::Matrix3x2F Transform2D::_CameraTM = D2D1::Matrix3x2F::Identity(); // ���� ��ǥ�� ��ȯ�� ���� ���

D2D1::Matrix3x2F Transform2D::_ScreenTM = D2D1::Matrix3x2F::Identity();


