#pragma once
#include "../Base/base_window.h"
class UiButton;

class DeadMessage :
    public BaseWindow
{
public:
	DeadMessage();
	~DeadMessage();
	static void ResetCharacter();
private:
	void ReadyWindow() override;
	void UpdateWindow() override;
	void RenderWinodw(HDC hdc) override;

private:

	std::shared_ptr<UiButton> _ok_button;
};

