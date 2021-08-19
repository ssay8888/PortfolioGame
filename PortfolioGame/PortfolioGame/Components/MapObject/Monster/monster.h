#pragma once
#include "../../Base/game_object.h"
class Player;
class AttackInfo;
class AiMovement;
class MonsterMovement;
class MonsterParts;
class FootHold;
class MonsterFrameManager;
class Monster :
	public GameObject
{

public:
	enum class MonsterState { kStand, kMove, kHit, kAttack1, kAttack2, kAttack3, kDie };
	Monster();
	~Monster();
	MonsterInfo GetMonsterInfo();

	void SetMonsterCode(std::string code);
	std::string GetMonsteCode() const;
	void SetMonsterId(int32_t code);
	int32_t GetMonsterId() const;

	void SetLevel(uint8_t level);
	uint8_t GetLevel() const;

	void SetSpeed(float speed);
	float GetSpeed() const;
	void SetUndead(uint8_t undead);
	uint8_t GetUndead() const;
	void SetBodyAttack(uint8_t body);
	uint8_t GetBodyAttack() const;
	void SetBoss(uint8_t boss);
	bool IsBoss() const;

	void SetPad(uint16_t pad);
	uint16_t GetPad() const;
	void SetMad(uint16_t mad);
	uint16_t GetMad() const;
	void SetPdd(uint16_t pdd);
	uint16_t GetPdd() const;
	void SetMdd(uint16_t mdd);
	uint16_t GetMdd() const;

	void SetExp(uint32_t exp);
	uint32_t GetExp() const;
	void SetHp(uint32_t hp);
	uint32_t GetHp() const;
	void GainHp(const int32_t hp);
	void SetMaxHp(uint32_t maxhp);
	uint32_t GetMaxHp() const;
	void SetMp(uint32_t mp);
	uint32_t GetMp() const;
	void GainMp(const int32_t mp);
	void SetMaxMp(uint32_t maxmp);
	uint32_t GetMaxMp() const;

	void SetPlayer(GameObject* player);
	GameObject* GetPlayer() const;
	void InsertAttackInfo(const std::string& key, AttackInfo* attack_info);
	AttackInfo* FindAttackInfo(std::string key) const;
	std::map<std::string, AttackInfo*>& GetAttackInfo();

	void SetMovement(std::shared_ptr<MonsterMovement> movement);
	std::shared_ptr<MonsterMovement> GetMovement() const;
	void SetName(std::string name);
	std::string GetName() const;

	void InsertAttackDelay(std::string key, uint64_t tick);
	uint64_t FindAttackDelay(std::string key);
	size_t GetAttackDelaySize() const;

	bool GetFacingDirection() const;
	bool IsAlive() const;
	void SetStateString(std::string str);
	std::string GetStateString() const;
	void ChangeState(MonsterState state);
	void AttackApply(std::string key);

	void SetAttackTick();
	uint64_t GetAttackTick()const;
	MonsterFrameManager* GetBaseFrame() const;

	void SetIsAttacking(bool g);
	bool IsAttacking() const;
private:
	virtual int ReadyGameObject() override;
	virtual void UpdateGameObject(const float deltaTime) override;
	virtual void RenderGameObject(HDC hdc) override;
	virtual void LateUpdateGameObject() override;

	void UpdateRect();
	
	void IsJumping();
	bool IsChangeFoothold();

	
private:
	MonsterInfo _monster_info;
	std::shared_ptr<MonsterMovement> _movement;
	std::string _monster_code;
	int32_t _monster_id;



	//몬스터상태관련
	FootHold* _now_foothold;
	MonsterState _monster_state;
	bool _facing_direction;
	bool _is_alive;
	std::string _state_string;

	//죽음 관련
	uint64_t _die_wait_tick;
	uint8_t _alpha_value;
	uint64_t _alpha_tick;
	bool _is_drop_item;

	//프레임관련변수
	MonsterFrameManager* _base_state_frame;
	std::vector<std::shared_ptr<MonsterParts>> _this_frame;

	//몬스터이동
	FootHold* _this_foothold;
	AiMovement* _ai_movement;
	GameObject* _target;

	//공격관련
	std::map<std::string, AttackInfo*> _attack_info;
	std::map<std::string, uint64_t> _attack_delay;
	uint64_t _attack_tick;
	bool _is_attack;

	HDC _memDC2;
	HBITMAP _bitmap2;
	HBITMAP _old_bitmap2;
};

