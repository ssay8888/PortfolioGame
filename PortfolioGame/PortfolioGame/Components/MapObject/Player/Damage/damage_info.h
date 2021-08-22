#pragma once
class GameObject;
class MyBitmap;
class DamageInfo
{
public:
	DamageInfo(GameObject* target, uint32_t damage, uint32_t delay, bool is_attack = true);
	DamageInfo(GameObject* target, std::list<uint32_t> damage, uint32_t delay, bool is_attack = true);
	~DamageInfo();
	void UpdateDamages();
	void RenderDamage(HDC hdc);
	bool IsAlive();
private:
	void ReadyDamage(bool is_attack);
private:
	GameObject* _targetObject;
	Info _info;
	std::list<uint32_t> _damages;
	uint32_t _delay;
	uint64_t _tick;
	bool _is_alive;
	std::list<std::pair<POINT, std::list<std::shared_ptr<MyBitmap>>>> _damage_image;
};

