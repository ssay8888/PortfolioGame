#pragma once
class MyBitmap;
class UiButton
{

public:
	enum class ButtonState { kNormal, kMouseOver, kPressed };
	UiButton();
	~UiButton();
	void ReadyButton(std::wstring buttonPath);
	void UpdateButton();
	void RenderButtonUi(HDC hdc);

	void SetObjectPos(Info pos);
	void SetObjectPosX(float x);
	void SetObjectPosY(float y);

private:
	Info _info{};
	RECT _rect{};
	ButtonState _state;

	std::shared_ptr<MyBitmap> _normalImage;
	std::shared_ptr<MyBitmap> _overImage;
	std::shared_ptr<MyBitmap> _pressedImage;


};

