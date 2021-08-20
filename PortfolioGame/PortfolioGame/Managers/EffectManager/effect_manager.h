#pragma once
class EffectInfo;
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
	void LoadEffectParts();

	void UpdateEffect();
	void RenderEffect(HDC hdc);
	void LateUpdateEffect();

	void InsertEffect(std::shared_ptr<EffectInfo> effect);
	void ShowEffect(std::string key);

	std::shared_ptr<MyBitmap> GetAttackDamageNumber(uint32_t number);
	std::shared_ptr<MyBitmap> GetTakeDamageNumber(uint32_t number);
private:
	
	std::list<std::shared_ptr<EffectInfo>> _show_effect_list;



	std::vector<std::shared_ptr<MyBitmap>> _attack_damage_unmber;
	std::vector<std::shared_ptr<MyBitmap>> _take_damage_unmber;
	std::map<std::string, std::shared_ptr<EffectInfo>> _list_effect;
};

