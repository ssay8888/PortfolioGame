#pragma once
#include "../Base/base_window.h"

class SkinInfo;
class Item;
class UiScroll;

class InventoryWindow :
	public BaseWindow
{
public:
	InventoryWindow();
	~InventoryWindow() override;

	void SetTab(ObjectType::InventoryTabState tab);
	ObjectType::InventoryTabState GetTab() const;
	void CharacterInventoryItemRender(HDC hdc);

	std::shared_ptr<Item> PointCollisionItem(POINT pos);
	std::shared_ptr<SkinInfo> PointCollisionEqp(POINT pos);
	std::shared_ptr<SkinInfo> PointCollisionEquipment(POINT pos);
private:
	void ReadyWindow() override;
	void UpdateWindow() override;
	void RenderWinodw(HDC hdc) override;
	void ScrollBarUp(POINT mouse);
	void ScrollBarDown(POINT mouse);
	void SelectItem(POINT mouse);
	void SelectItemMoveing(HDC hdc);
	void CancelSelectItem(POINT mouse);

	void EquipMove(POINT mouse);

	ObjectType::InventoryTabState FindTab();
private:
	ObjectType::InventoryTabState _this_tab;
	UiScroll* _scroll;
	const int _item_distance;
	bool _is_select_item;
	int32_t _select_position;
	std::shared_ptr<Item> _select_item;
	std::shared_ptr<SkinInfo> _select_eqp_item;
};


