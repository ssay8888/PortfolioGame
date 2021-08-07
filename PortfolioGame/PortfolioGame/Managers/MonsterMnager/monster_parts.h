#pragma once
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

	void SetImage(std::shared_ptr<MyBitmap*> image);
	std::shared_ptr<MyBitmap*> GetImage();

private:
	uint32_t _delay;
	POINT _origin;
	POINT _head;
	RECT _rect;
	std::shared_ptr<MyBitmap*> _image;
};

