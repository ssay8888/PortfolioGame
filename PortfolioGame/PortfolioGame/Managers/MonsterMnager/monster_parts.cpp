#include "../../pch.h"
#include "monster_parts.h"

MonsterParts::MonsterParts() :
	_delay(0),
	_origin({}),
	_head({}),
	_rect({}),
	_image(nullptr)
{

}

MonsterParts::~MonsterParts()
{
}

uint32_t MonsterParts::GetDelay() const
{
	return _delay;
}

void MonsterParts::SetDelay(uint32_t delay)
{
	_delay = delay;
}

POINT MonsterParts::GetOriginPos() const
{
	return _origin;
}

void MonsterParts::SetOriginPos(POINT origin)
{
	_origin = origin;
}

void MonsterParts::SetOriginPosX(LONG x)
{
	_origin.x = x;
}

void MonsterParts::SetOriginPosY(LONG y)
{
	_origin.y = y;
}

POINT MonsterParts::GetHeadPos() const
{
	return _head;
}

void MonsterParts::SetHeadPos(POINT headpos)
{
	_head = headpos;
}

void MonsterParts::SetHeadPosX(LONG x)
{
	_head.x = x;
}

void MonsterParts::SetHeadPosY(LONG y)
{
	_head.y = y;
}

RECT MonsterParts::GetRect() const
{
	return _rect;
}

void MonsterParts::SetRect(RECT rect)
{
	_rect = rect;
}

void MonsterParts::SetRectLeft(LONG left)
{
	_rect.left = left;
}

void MonsterParts::SetRectTop(LONG top)
{
	_rect.top = top;

}

void MonsterParts::SetRectRight(LONG right)
{
	_rect.right = right;
}

void MonsterParts::SetRectBottom(LONG bottom)
{
	_rect.bottom = bottom;
}

void MonsterParts::SetImage(std::shared_ptr<MyBitmap*> image)
{
	_image = image;
}

std::shared_ptr<MyBitmap*> MonsterParts::GetImage()
{
	return _image;
}
