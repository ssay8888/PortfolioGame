#include "../../pch.h"
#include "foot_hold.h"
#include "../ScrollManager/scroll_manager.h"

FootHold::FootHold() :
	_start({}),
	_end({}),
	_state(HoldState::kLine)
{
}

FootHold::~FootHold()
{
}

void FootHold::SetStartPos(const float x, const float y)
{
	_start.x = x;
	_start.y = y;
}

void FootHold::SetEndPos(const float x, const float y)
{
	_end.x = x;
	_end.y = y;
}

ObjectPos FootHold::GetStartPos() const
{
	return _start;
}

ObjectPos FootHold::GetEndPos() const
{
	return _end;
}

FootHold::HoldState FootHold::GetState() const
{
	return _state;
}

void FootHold::SetState(FootHold::HoldState state)
{
	_state = state;
}

void FootHold::RenderFootHold(HDC hdc)
{
	HPEN pen = nullptr;
	HPEN oldPen = nullptr;
	switch (_state)
	{
	case FootHold::HoldState::kLine:
		pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		break;
	case FootHold::HoldState::kRope:
		pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		break;
	case FootHold::HoldState::kLadder:
		pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		break;
	default:
		pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		break;
	}
	oldPen = (HPEN)SelectObject(hdc, pen);
	MoveToEx(hdc, static_cast<int>(_start.x + ScrollManager::GetScrollX()), static_cast<int>(_start.y + ScrollManager::GetScrollY()), nullptr);
	LineTo(hdc, static_cast<int>(_end.x + ScrollManager::GetScrollX()), static_cast<int>(_end.y + ScrollManager::GetScrollY()));
	SelectObject(hdc, oldPen);
	DeleteObject(pen);
	DeleteObject(oldPen);
}
