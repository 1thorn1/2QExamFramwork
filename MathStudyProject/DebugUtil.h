#pragma once
#include <D2D1Helper.h>

namespace debugUtil
{
	void MatrixToStr(D2D1::Matrix3x2F mat, WCHAR buffer[], const int length);

	#define DEBUGPRINT		printf
	#define DEBUGPRINTW		wprintf

}