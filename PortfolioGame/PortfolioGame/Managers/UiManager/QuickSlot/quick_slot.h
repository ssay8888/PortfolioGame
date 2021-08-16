#pragma once
class Item;
class MyBitmap;
class Skill;
class QuickSlot
{

public:
	enum class KeyBoard { kShift, kInsert, kHome, kPageUp, kControl, kDelete, kEndKey, kPageDown, kEnd };
	QuickSlot();
	~QuickSlot();

	void ReadyQuickSlot();
	void ChangeSlotSkill(KeyBoard key, Skill* skill);
	void ChangeSlotItem(KeyBoard key, std::shared_ptr<Item> skill);
	void UpdateQuickSlot();
	void RenderQuickSlot(HDC hdc);

	Skill* GetSkill(KeyBoard key) const;
	std::shared_ptr<Item> GetItem(KeyBoard key) const;
private:
	std::shared_ptr<MyBitmap> _background;
	Skill* _skills[8]{};
	std::shared_ptr<Item> _items[8]{};
};