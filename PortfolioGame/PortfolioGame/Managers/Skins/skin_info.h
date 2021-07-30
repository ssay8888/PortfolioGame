#pragma once
class SkinItem;
class SkinInfo
{
public:
	SkinInfo(std::string name);
	~SkinInfo() = default;
	void SetSkinItem(SkinItem* item);
	SkinItem* GetSkinItem();

	void SetName(const std::string name);
	std::string GetName() const;

	void SetDelay(uint32_t delay);
	uint32_t GetDelay() const;
private:
	SkinItem* _skin_item;
	std::string _name;
	uint32_t _delay;
};

