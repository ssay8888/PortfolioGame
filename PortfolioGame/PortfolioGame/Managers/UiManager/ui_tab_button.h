#pragma once
class MyBitmap;

class UiTabButton
{
public:
	enum class ButtonState { kDisable, kEnable };
	UiTabButton();
	~UiTabButton();
	void ReadyButton(std::wstring button_path);
	void UpdateButton();
	void RenderButtonUi(HDC hdc);
	void UpdateRect();

	ButtonState GetState() const;
	void SetState(ButtonState state);
	void SetInfoPos(Info pos);
	void SetInfoPosX(float x);
	void SetInfoPosY(float y);
	void SetShow(bool show);
	bool IsShow() const;
	RECT GetRect() const;
	void GainInfoPosX(float x);
	void GainInfoPosY(float y);
	void GainInfoPosXY(float x, float y);


protected:
	Info _info;
	RECT _rect;
	ButtonState _state;
	bool _show;

	std::shared_ptr<MyBitmap*> _enable;
	std::shared_ptr<MyBitmap*> _disable;

};

