#include "../../../pch.h"
#include "ui_scroll.h"

UiScroll::UiScroll() :
	_scroll_x(0),
	_scroll_y(0)
{
}

UiScroll::~UiScroll()
{
}

float UiScroll::GetScrollX() const
{
	return _scroll_x;
}

float UiScroll::GetScrollY() const
{
	return _scroll_y;
}

void UiScroll::SetScrollX(const float x)
{
	_scroll_x = x;
}

void UiScroll::SetScrollY(const float y)
{
	_scroll_y = y;
}

void UiScroll::GainScrollX(const float x)
{
	_scroll_x += x;
}

void UiScroll::GainScrollY(const float y)
{
	_scroll_y += y;
}
