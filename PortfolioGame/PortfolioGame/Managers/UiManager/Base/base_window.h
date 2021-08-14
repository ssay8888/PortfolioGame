#pragma once
class UiTabButton;
class MyBitmap;
class KeyManager;

class BaseWindow
{
public:
	BaseWindow();
	virtual ~BaseWindow();
	void DoReadyWindow();
	void DoUpdateWindow();
	void DoRenderWindow(HDC hdc);

	void BaseUiEvent();
	void BaseUiRender(HDC hdc);
	void SetShow(bool show);
	bool IsShow() const;


	bool InMouserSkillWindow() const;
protected:

	void BackgroundRender(HDC hdc) const;
	void TabRender(HDC hdc);
	void TitleBarClick(POINT mouse);
	void TitleBarMove(POINT mouse);
	void TitleBarUp(POINT mouse);
	void SelectTab(POINT mouse);
private:
	virtual void ReadyWindow() = 0;
	virtual void UpdateWindow() = 0;
	virtual void RenderWinodw(HDC hdc) = 0;
protected:
	Info _info;
	KeyManager* _key_manager;
	std::shared_ptr<MyBitmap> _background;
	std::vector<std::shared_ptr<UiTabButton>> _tab_buttons;
	bool _show;
	bool _move_window;
	POINT _click_pos;
};

