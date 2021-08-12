#pragma once
#include "monster_movement.h"
class MyBitmap;
class MonsterParts
{
public:
	MonsterParts();
	~MonsterParts();

	uint32_t GetDelay() const;
	void SetDelay(uint32_t delay);

	POINT GetOriginPos() const;
	void SetOriginPos(POINT origin);
	void SetOriginPosX(LONG x);
	void SetOriginPosY(LONG y);

	POINT GetHeadPos() const;
	void SetHeadPos(POINT headpos);
	void SetHeadPosX(LONG x);
	void SetHeadPosY(LONG y);

	RECT GetRect() const;
	void SetRect(RECT rect);
	void SetRectLeft(LONG left);
	void SetRectTop(LONG top);
	void SetRectRight(LONG right);
	void SetRectBottom(LONG bottom);

	std::string GetUol() const;
	void SetUol(std::string uol);

	void SetImage(std::shared_ptr<MyBitmap*> image);
	std::shared_ptr<MyBitmap*> GetImage() const;

	void SetPartner(std::shared_ptr<MonsterMovement*> partner);
	std::shared_ptr<MonsterMovement*> GetPartner() const;
private:
	uint32_t _delay;
	POINT _origin;
	POINT _head;
	RECT _rect;
	std::shared_ptr<MyBitmap*> _image;
	std::string _uol;
	std::shared_ptr<MonsterMovement*> _partner;
};

