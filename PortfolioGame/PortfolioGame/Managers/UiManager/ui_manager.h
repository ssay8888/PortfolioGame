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
	void UiLoad();

private:
	std::shared_ptr<MyBitmap*> _statusBar;
	std::shared_ptr<MyBitmap*> _quickSlot;

	std::map<std::string, std::shared_ptr<UiButton*>> _listButton;
};

