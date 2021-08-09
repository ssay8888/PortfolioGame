#pragma once
#include "../Base/game_object.h"
class MonsterMovement;
class MonsterParts;
class FootHold;
class Monster :
	public GameObject
{

public:
	enum class MonsterState { kStand, kMove, kDie };
	Monster();
	~Monster() = default;
	MonsterInfo GetMonsterInfo();

	void SetMonsterCode(std::string code);
	std::string GetMonsteCode() const;

	void SetLevel(uint8_t level);
	uint8_t GetLevel() const;

	void SetSpeed(float speed);
	float GetSpeed() const;
	void SetUndead(uint8_t undead);
	uint8_t GetUndead() const;
	void SetBodyAttack(uint8_t body);
	uint8_t GetBodyAttack() const;

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
	void SetMaxMp(uint32_t maxmp);
	uint32_t GetMaxMp() const;

	void SetMovement(std::shared_ptr<MonsterMovement*> movement);
	std::shared_ptr<MonsterMovement*> GetMovement();
	void SetName(std::string name);
	std::string GetName() const;

	bool GetFacingDirection();
	bool IsAlive() const;
private:
	virtual int ReadyGameObject() override;
	virtual void UpdateGameObject(const float deltaTime) override;
	virtual void RenderGameObject(HDC hdc) override;
	virtual void LateUpdateGameObject() override;
	
	void IsJumping();
	void ChangeState(MonsterState state);

private:
	MonsterInfo _monster_info;
	std::shared_ptr<MonsterMovement*> _movement;
	std::string _monster_code;



	//몬스터상태관련
	FootHold* _now_foothold;
	MonsterState _monster_state;
	bool _facing_direction;
	bool _is_alive;

	//프레임관련변수
	uint64_t _frame_tick;
	uint16_t _frame_nummber;
	bool _frame_revers;
	std::vector<std::shared_ptr<MonsterParts*>> _this_frame;


	HDC _memDC;
	HBITMAP _bitmap;
	HBITMAP _old_bitmap;
};

