#pragma once
class QuestSay
{
public:
	QuestSay();
	~QuestSay();


	void SetId(int32_t id);
	void SetSay(std::string say);
	void SetType(ObjectType::NpcTalkType type);

	int32_t GetId() const;
	std::string GetSay() const;
	ObjectType::NpcTalkType GetType() const;
private:
	int32_t _id;
	std::string _say;
	ObjectType::NpcTalkType _type;
};

