#pragma once
class UiScroll
{
public:
	UiScroll();
	~UiScroll();
	
	float GetScrollX() const;
	float GetScrollY() const;

	void  SetScrollX(float x);
	void  SetScrollY(float y);

	void  GainScrollX(float x);
	void  GainScrollY(float y);

private:
	float _scroll_x;
	float _scroll_y;
};

