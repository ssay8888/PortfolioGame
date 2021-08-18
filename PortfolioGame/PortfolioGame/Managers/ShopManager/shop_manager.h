#pragma once
class ShopItem;

class ShopManager
{
	ShopManager();
	~ShopManager();
public:
	static ShopManager* GetInstance()
	{
		static ShopManager instance;
		return &instance;
	}


	void InsertShop(int32_t shop_id, std::vector<std::shared_ptr<ShopItem>> items);
	std::vector<std::shared_ptr<ShopItem>> FindShop(int32_t shop_id);
	void LoadShopData();
private:
	std::map<int32_t, std::vector<std::shared_ptr<ShopItem>>> _shop;
};

