#pragma once
class SkillInfo
{
public:
	SkillInfo();
	~SkillInfo();
	int32_t GetX() const;
	int32_t GetY() const;
	int32_t GetAttackCount() const;
	int32_t GetMobCount() const;
	int32_t GetMad() const;
	int32_t GetMpCon() const;
	int32_t GetTime() const;
	int32_t GetPdd() const;
	RECT GetRect() const;
	void SetX(int32_t x);
	void SetY(int32_t y);
	void SetAttackCount(int32_t attack_count);
	void SetMobCount(int32_t mob_count);
	void SetMad(int32_t mad);
	void SetMpCon(int32_t mp_con);
	void SetTime(int32_t time);
	void SetPdd(int32_t pdd);
	void SetRect(RECT rect);

private:
	int32_t _x;
	int32_t _y;
	int32_t _attack_count;
	int32_t _mob_count;
	int32_t _mad;
	int32_t _mp_con;
	int32_t _time;
	int32_t _pdd;
	RECT _rect;
};

