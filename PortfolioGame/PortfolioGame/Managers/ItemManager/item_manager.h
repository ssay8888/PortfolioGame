#pragma once
class Item;
class ItemManager
{
private:
	ItemManager();
	~ItemManager();
public:
	static ItemManager* GetInstance()
	{
		static ItemManager instance;
		return &instance;
	}

	void LoadItem();
	void InsertItem(int32_t item_id, std::shared_ptr<Item> item);
	std::shared_ptr<Item> FindItem(int32_t item_id);
	std::shared_ptr<Item> FindCopyItem(int32_t item_id);

	std::map<int32_t, std::shared_ptr<Item>> _list_item;
};

