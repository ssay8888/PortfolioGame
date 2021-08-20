#pragma once
#include "../Base/base_window.h"
class UiButton;

class StatWindow final :
	public BaseWindow
{
public:
	StatWindow();
	~StatWindow() override;

	static void StrUp();
	static void DexUp();
	static void IntUp();
	static void LukUp();
	static void DetailBackGroundSlide();
public:
	void SetShowBackGround2(bool show);
	bool IsShowBackGround2() const;

private:
	void ReadyWindow() override;
	void UpdateWindow() override;
	void RenderWinodw(HDC hdc) override;

	void BaseStatWindowRender(HDC hdc);
	void BaseStatInfoRender(HDC hdc);
private:
	bool _show_background2;
	KeyManager* _key_manager2;
	std::shared_ptr<MyBitmap> _background2;
	std::shared_ptr<UiButton> _ap_up_button[4];
	std::shared_ptr<UiButton> _ap_up_auto_button;
	std::shared_ptr<UiButton> _detail_button;
};

