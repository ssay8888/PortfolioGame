#pragma once
#include "../../Base/game_object.h"
class MyBitmap;
class Item
{
public:
	Item();
	~Item();

	void SetIcon(std::shared_ptr<MyBitmap> icon);
	void InsertIconRaw(std::shared_ptr<MyBitmap> icon_raw);
	std::shared_ptr<MyBitmap> GetIcon() const;
	std::shared_ptr<MyBitmap> GetIconRaw() const;
	std::vector<std::shared_ptr<MyBitmap>> GetIconRaws() const;

	void SetPrice(int32_t price);
	int32_t GetPrice() const;
	void SetSlotMax(int32_t slot_max);
	int32_t GetSlotMax() const;
	void SetQuantity(int32_t quantity);
	void GainQuantity(int32_t quantity);
	void SetItemId(int32_t item_id);
	int32_t GetItemId() const;
	int32_t GetQuantity() const;
	void InsertSpec(const std::string& name, int32_t value);
	std::map<std::string, int32_t>& GetSpec();

	void SetIncMad(int32_t mad);
	void SetIncInt(int32_t mad);
	void SetSuccess(int32_t mad);
	int32_t GetIncMad() const;
	int32_t GetIncInt() const;
	int32_t GetSuccess() const;

	void SetDelayTick();
	bool IsNextFrameDelay() const;
	void AddFrameNumber();
	uint32_t GetFrameNumber()const;
private:
	std::shared_ptr<MyBitmap> _icon;
	std::vector<std::shared_ptr<MyBitmap>> _icon_raw;
	int32_t _price;
	int32_t _slot_max;
	int32_t _quantity;
	int32_t _item_id;
	std::map<std::string, int32_t> _spec;

	uint64_t _icon_raw_delay_tick;
	uint32_t _frame_number;


	int32_t _inc_mad;
	int32_t _inc_int;
	int32_t _success;
};

