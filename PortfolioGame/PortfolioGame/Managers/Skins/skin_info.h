#pragma once
class SkinItem;
class SkinParts;
class SkinFrame;
class SkinInfo
{
public:
	SkinInfo();
	SkinInfo(std::string name);
	~SkinInfo() = default;

	void InsertBodySkinItem(SkinItem* skinItem);
	void InsertHeadSkinItem(SkinItem* skinItem);
	SkinItem* FindBodySkinItem(std::string key);
	SkinItem* FindHeadSkinItem(std::string key);
	std::map<std::string, SkinItem*>* GetBodySkinItem();
	std::map<std::string, SkinItem*>* GetHeadSkinItem();

	void SetName(const std::string name);
	std::string GetName() const;


private:
	std::map<std::string, SkinItem*> _bodySkinItem;
	std::map<std::string, SkinItem*> _headSkinItem;
	std::string _name;
};

