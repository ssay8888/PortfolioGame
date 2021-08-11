#pragma once
class Skill;
class UiTabButton;
class MyBitmap;
class KeyManager;
class UiScroll;
class SkillWindow
{
public:
	enum class SkillTab { kBasic = 0, k1St, k2Nd, k3Nd, k4Th, kEnd };
	SkillWindow();
	~SkillWindow();

	void ReadyWindow();
	void UpdateWindow();


	void RenderWindow(HDC hdc);
	void RenderBackground(HDC hdc) const;
	void RenderSp(HDC hdc) const;
	void RenderSkillList(HDC hdc);
	void RenderTab(HDC hdc);
	void RenderQuickSlot(HDC hdc);

	void WindowEvent();
	void FindMouseOverSkillId(POINT mouse);
	void TitleBarClick(POINT mouse);
	void TitleBarMove(POINT mouse);
	void TitleBarUp(POINT mouse);
	void SelectTab(POINT mouse);

	void SelectSkillIcon(POINT mouse);
	void DropSkillIcon(POINT mouse);

	void ScrollBarUpClick(POINT mouse) const;
	void ScrollBarDownClick(POINT mouse) const;
	bool InMouserSkillWindow() const;

	void SetInfo(Info info);
	RECT GetRect() const;
	Info GetInfo() const;
private:
	void UpdateRect();
private:
	Info _info;
	RECT _rect{};
	MyBitmap* _skill_window_image;
	bool _show_window{};
	bool _move_window{};
	POINT _click_pos{};
	SkillTab _select_tab;
	std::list<UiTabButton*> _skill_window_buttons;
	std::list<Skill*> _show_skill_list;
	KeyManager* _key_manager;
	UiScroll* _scroll;
	uint32_t _mouse_over_skillid;

	const int _skill_ui_distance;

	Skill* _select_skill;
};

