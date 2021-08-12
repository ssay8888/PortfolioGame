#pragma once
class Player;
class Monster;
class Skill;

class MagicAttack
{
public:
	MagicAttack(Skill* skill, Player* player, std::list<Monster*> target);
	~MagicAttack();

public:
	void ReadyMagicAttack();
	void SkillUpdate() const;
	void SkillRender(HDC hdc);

	void BallSkill();
	void MagicSkill();

	void RunEffectImage(HDC hdc);
	void RunHitImage(HDC hdc);
	void RunBallImage(HDC hdc);

	void ResetSkill(Skill* skill, Player* _player, std::list<Monster*> _target);

	bool IsFinishedFrame();
	
private:
	Skill* _skill;
	std::list<Monster*> _targets;
	Player* _player;

	POINT _ball_pos;
	uint32_t _ball_frame_number;
	uint64_t _ball_frame_delay;
	bool _is_ball_frame_finished;

	uint32_t _effect_frame_number;
	uint64_t _effect_frame_delay;
	bool _is_effect_frame_finished;

	uint32_t _hit_frame_number;
	uint64_t _hit_frame_delay;
	bool _is_hit_frame_finished;

	uint64_t _finishied_delay_tick;


	HDC _memDC;
	HBITMAP _bitmap;
	HBITMAP _old_bitmap;
};

