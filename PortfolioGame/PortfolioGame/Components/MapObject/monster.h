#pragma once
#include "../Base/game_object.h"
class MonsterMovement;
class MonsterParts;
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

	void SetSpeed(uint8_t speed);
	uint8_t GetSpeed() const;
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


private:
	virtual int ReadyGameObject() override;
	virtual void UpdateGameObject(const float deltaTime) override;
	virtual void RenderGameObject(HDC hdc) override;
	virtual void LateUpdateGameObject() override;
	
	void ChangeState(MonsterState state);

private:
	MonsterInfo _monster_info;
	std::shared_ptr<MonsterMovement*> _movement;
	std::string _monster_code;



	MonsterState _monster_state;
	uint64_t _frame_tick;
	uint16_t _frame_nummber;
	std::vector<std::shared_ptr<MonsterParts*>> _this_frame;
};

