#pragma once
class MyBitmap;
class Item
{
public:
	Item();
	~Item();

	void SetIcon(std::shared_ptr<MyBitmap> icon);
	void SetIconRaw(std::shared_ptr<MyBitmap> icon_raw);
	std::shared_ptr<MyBitmap> GetIcon() const;
	std::shared_ptr<MyBitmap> GetIconRaw() const;

	void SetPrice(int32_t price);
	int32_t GetPrice() const;
	void SetSlotMax(int32_t slot_max);
	int32_t GetSlotMax() const;


	void SetQuantity(int32_t quantity);
	void GainQuantity(int32_t quantity);
	int32_t Getquantity() const;
	void InsertSpec(const std::string& name, int32_t value);
	std::map<std::string, int32_t>& GetSpec();
private:
	std::shared_ptr<MyBitmap> _icon;
	std::shared_ptr<MyBitmap> _icon_raw;
	int32_t _price;
	int32_t _slot_max;
	int32_t _quantity;
	std::map<std::string, int32_t> _spec;
};

