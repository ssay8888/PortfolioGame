#pragma once

class QuestSay;
class QuestReward;
class QuestCheck;

class QuestInfo
{
public:
	QuestInfo();
	~QuestInfo();

	void SetNpcId(int32_t id);
	void InsertSay(std::shared_ptr<QuestSay> say);
	void InsertSuccess(std::shared_ptr<QuestSay> success);
	void InsertIngSay(std::shared_ptr<QuestSay> say);
	void InsertReward(std::shared_ptr<QuestReward> reward);
	void InsertCheck(std::shared_ptr<QuestCheck> check);

	int32_t GetNpcId() const;
	std::vector<std::shared_ptr<QuestSay>>& GetSay();
	std::vector<std::shared_ptr<QuestSay>>& GetSuccess();
	std::vector<std::shared_ptr<QuestSay>>& GetIngSay();
	std::vector<std::shared_ptr<QuestReward>>& GetReward();
	std::vector<std::shared_ptr<QuestCheck>>& GetCheck();
private:
	int32_t _npc_id;
	std::vector<std::shared_ptr<QuestSay>> _say;
	std::vector<std::shared_ptr<QuestSay>> _success;
	std::vector<std::shared_ptr<QuestSay>> _ing_say;
	std::vector<std::shared_ptr<QuestReward>> _reward;
	std::vector<std::shared_ptr<QuestCheck>> _check;
};

