#pragma once
class Skill;

class MagicAttack
{
public:
	MagicAttack();
	~MagicAttack();

public:

	
private:
	Skill* _skill;
	uint64_t _frame_delay;
};

