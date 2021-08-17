#pragma once
#include "NpcTalk/npc_talk_window.h"
class StatWindow;
class InventoryWindow;
class SkillWindow;
class MyBitmap;
class UiButton;
class QuickSlot;
class EquipmentWindow;
class ItemInfoToolTip;
class UiManager
{
	UiManager();
	~UiManager();
public:
	static UiManager* GetInstance();

	void UpdateUiManager();
	void ReadyUiManager();
	void RenderUiManager(HDC hdc);

	std::shared_ptr<QuickSlot> GetQuickSlot() const;
	std::shared_ptr<InventoryWindow> GetInventoryWindow() const;
	std::shared_ptr<EquipmentWindow> GetEquipmentWindow() const;
	std::shared_ptr<StatWindow> GetStatWindow() const;
	std::shared_ptr<NpcTalkWindow> GetNpcTalkWindow() const;
private:
	void ButtonUiLoad();
	void PlayerInfoUiLoad();
	void StatusGageBarRender(HDC hdc);
	void StatusLevelRender(HDC hdc);

private:
	std::shared_ptr<MyBitmap> _status_bar;
	std::shared_ptr<QuickSlot> _quick_slot;
	std::map<std::string, std::shared_ptr<UiButton>> _list_button;
	std::map<uint8_t, std::shared_ptr<MyBitmap>> _list_level;
	std::shared_ptr<MyBitmap> _hp_bar;
	std::shared_ptr<MyBitmap> _mp_bar;
	std::shared_ptr<MyBitmap> _exp_bar;

	std::shared_ptr<SkillWindow> _skill_window;
	std::shared_ptr<InventoryWindow> _inventory_window;
	std::shared_ptr<EquipmentWindow> _equipment_window;
	std::shared_ptr<StatWindow> _stat_window;
	std::shared_ptr<ItemInfoToolTip> _item_info_tool_tip;
	std::shared_ptr<NpcTalkWindow> _npc_talk_window;

};