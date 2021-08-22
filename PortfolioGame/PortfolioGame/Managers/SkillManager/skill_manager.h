#pragma once
class BuffUi;
class Skill;
class SkillManager
{
	SkillManager();
	~SkillManager();
public:
	static SkillManager* GetInstance();
	Skill* FindSkill(uint32_t skill_id);
	std::list<Skill*> FindSkillList(uint32_t job_id);
	void InsertSkill(int32_t skill_id, Skill* skill);

	void InsertBuffSkill(int32_t skill_id);
	std::shared_ptr<BuffUi> FindBuffSkill(int32_t skill_id);
	void UpdateBuff();
	void RenderBuff(HDC hdc);
public:
	static void LoadSkillData();
private:
	std::map<uint32_t, Skill*> _skills;
	std::list<std::shared_ptr<BuffUi>> _buff_list;
};

