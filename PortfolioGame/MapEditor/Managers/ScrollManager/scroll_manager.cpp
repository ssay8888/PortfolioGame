#include "../../pch.h"
#include "scroll_manager.h"

float ScrollManager::_scrollX = 0;
float ScrollManager::_scrollY = 0;

void ScrollManager::Scroll_Lock()
{

	//if (0 < _scrollX)
	//	m_iScrollX = 0;
	//if (0 < _scrollY)
	//	m_iScrollY = 0;

	//if (WINCX - TILECX * TILEX > _scrollX)
	//	m_iScrollX = WINCX - TILECX * TILEX;
	//if (WINCY - TILECY * TILEY > _scrollY)
	//	m_iScrollY = WINCY - TILECY * TILEY;
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

void ScrollManager::GainScrollX(float x)
{
	_scrollX += x;
}

void ScrollManager::GainScrollY(float y)
{
	_scrollY += y;
}
