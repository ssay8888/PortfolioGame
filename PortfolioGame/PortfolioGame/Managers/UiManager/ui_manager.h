#pragma once
class MyBitmap;
class UiButton;
class UiManager
{
	UiManager() = default;
	~UiManager() = default;

public:
	static UiManager* GetInstance()
	{
		static UiManager instance;
		return &instance;
	}

	void UpdateUiManager();
	void ReadyUiManager();
	void RednerUiManager(HDC hdc);

private:
	void ButtonUiLoad();
	void PlayerInfoUiLoad();
	void StatusGageBarRender(HDC hdc);
	void StatusLevelRender(HDC hdc);
private:
	std::shared_ptr<MyBitmap*> _statusBar;
	std::shared_ptr<MyBitmap*> _quickSlot;

	std::map<std::string, std::shared_ptr<UiButton*>> _listButton;

	std::map<uint8_t, std::shared_ptr<MyBitmap*>> _listLevel;

	std::shared_ptr<MyBitmap*> _hpBar;
	std::shared_ptr<MyBitmap*> _mpBar;
	std::shared_ptr<MyBitmap*> _expBar;


};

