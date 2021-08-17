#pragma once
class QuestReward
{
public:
	QuestReward();
	~QuestReward();

	void SetRewardType(std::string type);
	std::string GetRewardType() const;
	void SetItemId(int32_t id);
	int32_t GetItemId() const;
	void SetValue(int32_t value);
	int32_t GetValue() const;
private:
	std::string _reward_type;
	int32_t _item_id;
	int32_t _value;
};

