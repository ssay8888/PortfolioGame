#include "../../pch.h"
#include "foot_hold.h"

FootHold::FootHold() :
	_start({}),
	_end({})
{
}

FootHold::~FootHold()
{
}

void FootHold::SetStartPos(const uint32_t x, const uint32_t y)
{
	_start.x = x;
	_start.y = y;
}

void FootHold::SetEndPos(const uint32_t x, const uint32_t y)
{
	_end.x = x;
	_end.y = y;
}

POINT FootHold::GetStartPos() const
{
	return _start;
}

POINT FootHold::GetEndPos() const
{
	return _end;
}

void FootHold::RenderFootHold(HDC hdc)
{
	HPEN pen = CreatePen(PS_SOLID , 2, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	MoveToEx(hdc, _start.x, _start.y, nullptr);
	LineTo(hdc, _end.x, _end.y);
	SelectObject(hdc, oldPen);
	DeleteObject(pen);
	DeleteObject(oldPen);
}
