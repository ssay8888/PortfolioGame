#include "../../pch.h"
#include "scroll_manager.h"

float ScrollManager::_scrollX = 0;
float ScrollManager::_scrollY = 0;

float ScrollManager::GetScrollX()
{
	return _scrollX;
}

float ScrollManager::GetScrollY()
{
	return _scrollY;
}

void ScrollManager::SetScrollX(float x)
{
	_scrollX = x;
}

void ScrollManager::SetScrollY(float y)
{
	_scrollY = y;
}

void ScrollManager::GainScrollX(float x)
{
	_scrollX += x;
}

void ScrollManager::GainScrollY(float y)
{
	_scrollY += y;
}
