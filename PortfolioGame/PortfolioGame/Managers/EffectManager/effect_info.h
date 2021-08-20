#pragma once
class EffectParts;

class EffectInfo
{
public:
	EffectInfo();
	~EffectInfo();

	void InsertEffectParts(std::shared_ptr<EffectParts> parts);
	std::vector<std::shared_ptr<EffectParts>>& GetListImage();

	bool UpdateFrame();
	void RenderFrame(HDC hdc);
	bool LateUpdateFrame();

	void SetFrameTick(uint64_t tick);
	void SetFrameNumber(int32_t number);
	void SetName(std::string name);
	uint64_t GetFrameTick() const;
	int32_t GetFrameNumber() const;
	std::string GetName() const;

private:
	std::vector<std::shared_ptr<EffectParts>> _list_image;
	uint64_t _frame_tick;
	int32_t _frame_number;
	std::string _name;
};

