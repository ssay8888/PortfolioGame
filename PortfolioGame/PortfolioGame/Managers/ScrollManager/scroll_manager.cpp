#include "../../pch.h"
#include "scroll_manager.h"
#include "../MapManager/map_manager.h"
#include "../MapManager/Map/map_instance.h"

float ScrollManager::_scrollX = 0;
float ScrollManager::_scrollY = 0;

void ScrollManager::ScrollLock()
{
	auto now_map = MapManager::GetInstance()->GetNowMap();
	if (now_map == nullptr) {
		return;
	}
	if (0 < _scrollX)
		_scrollX = 0;
	if (0 < _scrollY)
		_scrollY = 0;

	if (WindowCX - now_map->GetMapSize().x > _scrollX)
		_scrollX = WindowCX - static_cast<float>(now_map->GetMapSize().x);
	if (WindowCY - now_map->GetMapSize().y > _scrollY)
		_scrollY = WindowCY - static_cast<float>(now_map->GetMapSize().y);
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
	_scrollX += x;

}

void ScrollManager::GainScrollY(float y, bool temp)
{
	_scrollY += y;

}
