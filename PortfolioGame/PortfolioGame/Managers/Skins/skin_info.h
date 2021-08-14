#pragma once
class MyBitmap;
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

	void SetIcon(std::shared_ptr<MyBitmap*>);
	void SetIconRaw(std::shared_ptr<MyBitmap*>);
	std::shared_ptr<MyBitmap*> GetIcon() const;
	std::shared_ptr<MyBitmap*> GetIconRaw() const;

private:
	std::map<std::string, SkinItem*> _bodySkinItem;
	std::map<std::string, SkinItem*> _headSkinItem;
	std::shared_ptr<MyBitmap*> _icon;
	std::shared_ptr<MyBitmap*> _icon_raw;
	std::string _name;
};

