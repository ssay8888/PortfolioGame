#pragma once
class ScrollManager
{
public:
	ScrollManager() = default;
	~ScrollManager() = default;
	static void ScrollLock();
public:
	static float GetScrollX();
	static float GetScrollY();

	static void  SetScrollX(float x);
	static void  SetScrollY(float y);

	static void  GainScrollX(float x, bool temp = false);
	static void  GainScrollY(float y, bool temp = false);

private:
	static float _scrollX;
	static float _scrollY;
	static float _tempScrollX;
	static float _tempScrollY;
};

