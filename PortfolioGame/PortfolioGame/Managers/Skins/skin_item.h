#pragma once
class SkinFrame;
class SkinItem
{
public:
	SkinItem();
	~SkinItem();
	void AddFrame(SkinFrame* frame);
	SkinFrame* GetFrame(std::string) const;
	std::string GetName() const;
	void SetName(const std::string name);
private:
	std::string _name;
	std::map<std::string, SkinFrame*> _frames;
};

