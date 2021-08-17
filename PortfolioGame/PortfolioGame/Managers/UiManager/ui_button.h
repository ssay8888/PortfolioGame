#pragma once
#include <functional>
class KeyManager;
class MyBitmap;
class UiButton
{

public:
	enum class ButtonState { kDisable, kNormal, kMouseOver, kPressed };
	UiButton(KeyManager* key);
	~UiButton();
	void ReadyButton(std::wstring buttonPath);
	void UpdateButton();
	void RenderButtonUi(HDC hdc);
	void UpdateRect();
	ButtonState GetState() const;

	void SetObjectPos(Info pos);
	void SetObjectPosX(float x);
	void SetObjectPosY(float y);
	void GainObjectPosXY(float x, float y);
	void SetState(ButtonState state);
	float GetObjectPosX() const;
	float GetObjectPosY() const;
	int GetWidth() const;
	int GetHeight() const;
	void SetCallBack(std::function<void()> _method);
	std::function<void()> GetCallBack();
private:
	Info _info{};
	RECT _rect{};
	ButtonState _state;

	std::shared_ptr<MyBitmap> _disableImage;
	std::shared_ptr<MyBitmap> _normalImage;
	std::shared_ptr<MyBitmap> _overImage;
	std::shared_ptr<MyBitmap> _pressedImage;
	std::function<void()> _call_back;
	KeyManager* _key_manager;


};

