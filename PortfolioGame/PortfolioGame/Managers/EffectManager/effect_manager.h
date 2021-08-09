#pragma once
class MyBitmap;
class EffectManager
{

public:
	static EffectManager* GetInstance()
	{
		static EffectManager instance;
		return &instance;
	}


	void LoadDamageNumber();

	std::shared_ptr<MyBitmap*> GetAttackDamageNumber(uint32_t number);
	std::shared_ptr<MyBitmap*> GetTakeDamageNumber(uint32_t number);
private:
	std::vector<std::shared_ptr<MyBitmap*>> _attack_damage_unmber;
	std::vector<std::shared_ptr<MyBitmap*>> _take_damage_unmber;
};

