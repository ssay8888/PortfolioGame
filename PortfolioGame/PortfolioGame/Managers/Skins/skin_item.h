#pragma once
class SkinFrame;
class SkinInfo;
class SkinItem
{
public:
	SkinItem();
	~SkinItem();

	void InsertFrame(SkinFrame* frame);
	SkinFrame* FindFrame(std::string index) const;

	std::string GetName() const;
	void SetName(const std::string name);

	size_t GetFrameSize() const;
	std::vector<SkinFrame*>* GetSkinFrames();

	void SetPartner(SkinInfo* partner);
	SkinInfo* GetPartner() const;
private:
	std::string _name;
	std::vector<SkinFrame*> _skinFrames;

	SkinInfo* _partner;
};

