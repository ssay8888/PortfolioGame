#pragma once
class DropDataInfo
{
public:
	DropDataInfo();
	~DropDataInfo();

	void SetItemId(int32_t item_id);
	void SetRate(int32_t rate);
	void SetMinPrice(int32_t min_price);
	void SetMaxPrice(int32_t max_price);

	int32_t GetItemId() const;
	int32_t GetRate() const;
	int16_t GetMinPrice() const;
	int16_t GetMaxPrice() const;
	

private:
	int32_t _item_id;
	int32_t _rate;
	int16_t _min_price;
	int16_t _max_price;
};

