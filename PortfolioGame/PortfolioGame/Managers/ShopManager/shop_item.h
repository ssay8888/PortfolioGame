#pragma once
class ShopItem
{
public:
	ShopItem();
	~ShopItem();

	void SetItemId(int32_t item_id);
	void SetPrice(int32_t price);
	void SetMeso(int32_t meso);

	int32_t GetItemId() const;
	int32_t GetPrice() const;
	int32_t GetMeso() const;

private:
	int32_t _item_id;
	int32_t _price;
	int32_t _meso;
};

