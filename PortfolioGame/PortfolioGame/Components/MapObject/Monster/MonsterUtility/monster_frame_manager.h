#pragma once
class MonsterParts;

class MonsterFrameManager
{
public:
	MonsterFrameManager();
	~MonsterFrameManager();

	uint64_t GetFrameTick() const;
	uint64_t GetFrameNumber() const;
	bool IsFrameRevers() const;

	void SetFrameTick(uint64_t tick);
	void SetFrameNumber(uint16_t number);
	void SetFrameRevers(bool revers);
	void SetThisFrame(std::vector<std::shared_ptr<MonsterParts*>> frame);
	void ResetFrame();
	bool NextFrame(bool revers = true);
	bool IsFrameFinish() const;
	size_t GetFrameSize() const;
private:
	uint64_t _frame_tick;
	uint16_t _frame_number;
	bool _frame_revers;

	bool _frame_finish;
	std::vector<std::shared_ptr<MonsterParts*>> _this_frame;
};

