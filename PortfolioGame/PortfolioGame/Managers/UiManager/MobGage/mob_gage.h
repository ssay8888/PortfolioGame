#pragma once
class MyBitmap;
class Monster;

class MobGage
{
public:
	MobGage();
	~MobGage();

	void SetMonster(Monster* monster);
	Monster* GetMonster() const;

	void SetIcon(const std::shared_ptr<MyBitmap>& icon);
	std::shared_ptr<MyBitmap> GetIcon() const;

	void SetBaseBar(const std::shared_ptr<MyBitmap>& base_bar);
	std::shared_ptr<MyBitmap> GetBaseBar() const;

	void SetHpBar(const std::shared_ptr<MyBitmap>& hp_bar);
	std::shared_ptr<MyBitmap> GetHpBar() const;


	void ReadyMobGate();
	void UpdateMobGate();
	void RenderMobGage(HDC hdc);
	
private:
	Monster* _monster;
	std::shared_ptr<MyBitmap> _icon;
	std::shared_ptr<MyBitmap> _base_bar;
	std::shared_ptr<MyBitmap> _hp_bar;

};

