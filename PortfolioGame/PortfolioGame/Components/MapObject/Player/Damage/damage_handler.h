#pragma once
class MyBitmap;
class DamageInfo;
class GameObject;
class DamageHandler
{
public:
	DamageHandler() = default;
	~DamageHandler() = default;


	void InsertAttackDamageEffect(GameObject* target, uint32_t damage, uint32_t delay);
	void InsertTakeDamageEffect(GameObject* target, uint32_t damage, uint32_t delay);
	void ShowDamages(HDC hdc);
	void UpdateDamages();
private:
	std::list<DamageInfo*> _show_damage_list;
};