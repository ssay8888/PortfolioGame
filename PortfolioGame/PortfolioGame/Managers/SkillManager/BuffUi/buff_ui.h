#pragma once
class Skill;

class BuffUi
{
public:
	BuffUi(Skill* skill);
	~BuffUi();
	bool UpdateBuffUi();
	void RenderBuffUi(HDC hdc, POINT pos);
	Skill* GetSkill() const;
private:
	int64_t _start_tick;
	Skill* _skill;
	bool _is_flicker;
	bool _is_dead;
};

