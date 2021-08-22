#pragma once
class Skill;
class SkinInfo;
class Item;

class ItemInfoToolTip
{
public:
	ItemInfoToolTip();
	~ItemInfoToolTip();
	void BaseToolToolTipRender(HDC hdc);

private:
	/// <summary>
	/// 장비템제외 아이템툴팁관련
	/// </summary>
	void AlphaBlueScreenItemInfo(HDC hdc) const;
	void AlphaItemWhiteMiniScreen(HDC hdc) const;
	void SelectItemRender(HDC hdc, std::shared_ptr<Item> item) const;
	/// <summary>
	/// 장비템 툴팁
	/// </summary>
	void AlphaBlueScreenEquipIcon(HDC hdc) const;
	void AlphaEquipWhiteMiniScreen(HDC hdc) const;
	void SelectEquipRedner(HDC hdc, std::shared_ptr<SkinInfo> item) const;

	/// <summary>
	/// 스킬툴팁
	/// </summary>
	void SelectSkillRender(HDC hdc, Skill* skill) const;

	void AlphaBlueScreenEquipInfo(HDC hdc, std::shared_ptr<SkinInfo> item) const;
	std::pair<int32_t, std::wstring> SelectEquipInfo(std::shared_ptr<SkinInfo> item) const;
private:
	HDC _memDC;
	HBITMAP _bitmap;
	HBITMAP _old_bitmap;
	HDC _memDC2;
	HBITMAP _bitmap2;
	HBITMAP _old_bitmap2;
	HDC _memDC3;
	HBITMAP _bitmap3;
	HBITMAP _old_bitmap3;
	int count;
};

