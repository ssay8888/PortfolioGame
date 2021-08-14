#pragma once
class MonsterFrameManager;
class MonsterParts;
class MyBitmap;

class AttackInfo
{
public:
	AttackInfo();
	~AttackInfo();

	void SetAttackAfter(int32_t after);
	void SetEffectAfter(int32_t after);
	void SetAttackAfterTick(int64_t after);
	void SetEffectAfterTick(int64_t after);
	void SetMpCon(int32_t after);
	void SetDeadlyAttack(bool is_deadly_attack);
	void SetEffect(std::vector<std::shared_ptr<MonsterParts*>> effect);
	void SetHitEffect(const std::vector<std::shared_ptr<MonsterParts*>> effect);
	void SetAreaWarning(const std::vector<std::shared_ptr<MonsterParts*>> effect);
	void SetRange(RECT range);
	void SetType(int32_t type);
	void SetEffectFinish(bool check);
	void SetAttackFinish(bool check);

	int32_t GetAttackAfter() const;
	int64_t GetAttackAfterTick() const;
	int32_t GetEffectAfter() const;
	int64_t GetEffectAfterTick() const;
	int32_t GetConMp() const;
	int32_t IsDeadlyAttack() const;
	bool IsAttackFinish() const;
	bool IsEffectFinish() const;
	RECT GetRange() const;
	std::vector<std::shared_ptr<MonsterParts*>>& GetAreaWarning();
	std::vector<std::shared_ptr<MonsterParts*>>& GetEffect();
	std::vector<std::shared_ptr<MonsterParts*>>& GetHitEffect();
	int32_t GetType() const;
	MonsterFrameManager* GetAreaWarningFrame() const;
	MonsterFrameManager* GetEffectFrame() const;
	MonsterFrameManager* GetHitEffectFrame() const;

	void ResetEffectFrame();
	void ResetAreaFrame();
	void InsertAreaAttack(POINT pos);
	std::list<POINT>& GetAreaAttack();

	bool IsAttack();
private:
	int32_t _attack_after;
	int64_t _attack_after_tick;
	bool _is_attack_finish;

	int32_t _effect_after;
	int64_t _effect_after_tick;
	bool _is_effect_finish;

	int32_t _con_mp;
	bool _is_deadly_attack;
	std::vector<std::shared_ptr<MonsterParts*>> _area_warning;
	std::vector<std::shared_ptr<MonsterParts*>> _effect;
	std::vector<std::shared_ptr<MonsterParts*>> _hit_effect;
	RECT _range;
	int32_t _type;
	MonsterFrameManager* _area_warning_frame;
	MonsterFrameManager* _effect_frame;
	MonsterFrameManager* _hit_effect_frame;

	std::list<POINT> _area_attack;

};

