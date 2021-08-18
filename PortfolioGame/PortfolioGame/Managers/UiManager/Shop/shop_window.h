#pragma once
#include "../Base/base_window.h"
class ShopItem;
class UiButton;
class SkinInfo;
class Item;
class UiScroll;

class ShopWindow :
	public BaseWindow
{
public :
	ShopWindow();
	~ShopWindow();
	void CharacterInventoryItemRender(const HDC hdc);
	void SetTab(const ObjectType::InventoryTabState tab);

	ObjectType::InventoryTabState GetTab() const;

	std::shared_ptr<Item> GetSelectItem() const;
	std::shared_ptr<SkinInfo> GetSelectEqp() const;
	std::shared_ptr<ShopItem> GetSelectBuyItem() const;
	void SetSelectItem(std::shared_ptr<Item> item);
	void SetSelectEqp(std::shared_ptr<SkinInfo> item);
	static void ExitShop();
	static void SellItemShop();
	static void BuyItemShop();
	void LoadShopData(int32_t shop_id);
private:

	void ReadyWindow() override;
	void UpdateWindow() override;
	void RenderWinodw(HDC hdc) override;

	ObjectType::InventoryTabState FindTab();

	void RenderShopItems(HDC hdc);
	void SelectBuyItem(POINT mouse);

	void RenderMeso(HDC hdc);
	void SelectItem(POINT mouse);
	void InvenScrollUp(POINT mouse);
	void InvenScrollDown(POINT mouse);
	void ShopScrollUp(POINT mouse);
	void ShopScrollDown(POINT mouse);
	void RenderShopButton(HDC hdc);
	void UpdateShopButton(POINT mouse);
	void ResetShop();
private:
	ObjectType::InventoryTabState _this_tab;
	std::shared_ptr<MyBitmap> _select_image;
	UiScroll* _inven_scroll;
	UiScroll* _shop_scroll;
	const int _item_distance;

	std::shared_ptr<ShopItem> _select_buy_item;
	std::shared_ptr<Item> _select_item;
	std::shared_ptr<SkinInfo> _select_eqp;
	std::shared_ptr<UiButton> _shop_exit_button;
	std::shared_ptr<UiButton> _shop_sell_button;
	std::shared_ptr<UiButton> _shop_buy_button;
	KeyManager* _buttonKeyManager;

	std::vector<std::shared_ptr<ShopItem>> _shop_items;
};

