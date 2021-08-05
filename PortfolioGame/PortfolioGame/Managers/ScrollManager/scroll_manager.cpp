#include "../../pch.h"
#include "scroll_manager.h"
#include "../MapManager/map_manager.h"

float ScrollManager::_scrollX = 0;
float ScrollManager::_scrollY = 0;
float ScrollManager::_tempScrollX = 0;
float ScrollManager::_tempScrollY = 0;

void ScrollManager::ScrollLock()
{
	if (0 < _scrollX)
		_scrollX = 0;
	if (0 < _scrollY)
		_scrollY = 0;

	if (WindowCX - MapManager::GetInstance()->GetMapSize().x > _scrollX)
		_scrollX = WindowCX - static_cast<float>(MapManager::GetInstance()->GetMapSize().x);
	if (WindowCY - MapManager::GetInstance()->GetMapSize().y > _scrollY)
		_scrollY = WindowCY - static_cast<float>(MapManager::GetInstance()->GetMapSize().y);
}

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

void ScrollManager::GainScrollX(float x, bool temp)
{
	if (temp)
	{
		_tempScrollX += x;
	}
	else if (_tempScrollX != 0)
	{
		_tempScrollX += x;
	}
	else
	{
		_scrollX += x;
	}
}

void ScrollManager::GainScrollY(float y, bool temp)
{
	if (temp)
	{
		_tempScrollY += y;
	}
	else if (_tempScrollY != 0)
	{
		_tempScrollY += y;
	}
	else
	{
		_scrollY += y;
	}
}
