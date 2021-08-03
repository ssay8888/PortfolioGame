#pragma once
class SkinParts;
class SkinItem;
class SkinFrame
{
public:
	SkinFrame();
	~SkinFrame();
	SkinParts* FindParts(std::string key);
	void InsertParts(std::string, SkinParts*);
	std::map<std::string, SkinParts*>* GetParts();

	uint32_t GetDelay() const;
	void SetDelay(uint32_t delay);

	std::string GetFrame() const;
	void SetFrame(std::string frame);

	uint32_t GetActionFrame() const;
	void SetActionFrame(uint32_t nummber);

	std::string GetAction() const;
	void SetAction(std::string action);

	void SetPartner(SkinItem* partner);
	SkinItem* GetPartner() const;
private:
	std::map<std::string, SkinParts*> _parts;
	uint32_t _delay;
	std::string _action;
	uint16_t _actionFrame;
	std::string _frame;

	SkinItem* _partner;
};

