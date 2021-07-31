#pragma once
class ScrollManager
{
public:
	ScrollManager() = default;
	~ScrollManager() = default;
public:
	static float GetScrollX();
	static float GetScrollY();

	static void  SetScrollX(float x);
	static void  SetScrollY(float y);

	static void  GainScrollX(float x);
	static void  GainScrollY(float y);

private:
	static float _scrollX;
	static float _scrollY;
};

