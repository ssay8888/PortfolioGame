#pragma once
class Skill;
class SkillManager
{
	SkillManager();
	~SkillManager();
public:
	static SkillManager* GetInstance();
	Skill* FindSkill(uint32_t skill_id);
	std::list<Skill*> FindSkillList(uint32_t job_id);
	static void LoadSkillData();
	void InsertSkill(int32_t skill_id, Skill* skill);
private:
	std::map<uint32_t, Skill*> _skills;
};

