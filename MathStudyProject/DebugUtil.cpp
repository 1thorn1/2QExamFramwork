
#include "DebugUtil.h"

#include <stdio.h>



namespace debugUtil
{
	void MatrixToStr(D2D1::Matrix3x2F mat, WCHAR buffer[], const int length)
	{
		swprintf_s(buffer, length,L" { %.1f, %.1f }\n { %.1f, %.1f }\n { %.1f, %.1f } \n ",
						mat._11, mat._12, mat._21, mat._22, mat._31, mat._32);

	}
}
