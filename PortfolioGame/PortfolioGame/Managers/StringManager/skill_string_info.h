#pragma once
class SkillStringInfo
{
public:
	SkillStringInfo();
	~SkillStringInfo();

	void SetSkillId(int32_t skill_id);
	int32_t GetSkillId() const;
	void SetName(std::wstring name);
	std::wstring GetName() const;
	void SetDesc(std::wstring desc);
	std::wstring GetDesc() const;

	void InsertInfoString(int32_t key, std::wstring value);
	std::wstring FindInfoString(int32_t key) const;
private:
	int32_t _skill_id;
	std::wstring _name;
	std::wstring _desc;
	std::map<int32_t, std::wstring> _info;
};

