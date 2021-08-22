#pragma once
class SkillStringInfo;
class StringInfo;
class StringManager
{
	StringManager();
	~StringManager();
public:
	static StringManager* GetInstance()
	{
		static StringManager instance;
		return &instance;
	}

	void InsertStringInfo(StringInfo info);
	void InsertSkillStringInfo(SkillStringInfo info);
	void LoadStringInfo();
	StringInfo FindItemStringInfo(int32_t item_id);
	SkillStringInfo FindSkillStringInfo(int32_t item_id);

private:
	std::map<int32_t, StringInfo> _list;
	std::map<int32_t, SkillStringInfo> _skill_list;
};

