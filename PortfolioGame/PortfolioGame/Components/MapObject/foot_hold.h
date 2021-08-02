#pragma once
class FootHold
{
public:
	FootHold();
	~FootHold();
	void SetStartPos(const uint32_t x, const uint32_t y);
	void SetEndPos(const uint32_t x, const uint32_t y);
	POINT GetStartPos() const;
	POINT GetEndPos() const;
	void RenderFootHold(HDC hdc);
private:
	POINT _start;
	POINT _end;
};

