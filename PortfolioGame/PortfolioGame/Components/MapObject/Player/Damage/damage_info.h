#pragma once
class GameObject;
class MyBitmap;
class DamageInfo
{
public:
	DamageInfo(GameObject* target, uint32_t damage, uint32_t delay);
	~DamageInfo();
	void UpdateDamages();
	void RenderDamage(HDC hdc);
	bool IsAlive();
private:
	void ReadyDamage();
private:
	GameObject* _targetObject;
	uint32_t _damage;
	uint32_t _delay;
	uint64_t _tick;
	bool _is_alive;
	std::list<std::shared_ptr<MyBitmap*>> _damage_image;
	POINT _position;
};

