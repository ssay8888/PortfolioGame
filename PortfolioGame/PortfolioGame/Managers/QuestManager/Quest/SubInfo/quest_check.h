#pragma once
class QuestCheck
{
public:
	QuestCheck();
	~QuestCheck();

	void SetItemId(int32_t id);
	void SetPrice(int32_t price);

	int32_t GetItemId() const;
	int32_t GetPrice() const;
private:
	int32_t _item_id;
	int32_t _price;
};

