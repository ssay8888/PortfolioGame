#pragma once
class FootHold
{
public:
	enum class HoldState { kLine, kRope, kLadder };
	FootHold();
	~FootHold();
	void SetStartPos(const float x, const float y);
	void SetEndPos(const float x, const float y);
	ObjectPos GetStartPos() const;
	ObjectPos GetEndPos() const;
	HoldState GetState() const;
	void SetState(HoldState state);
	void RenderFootHold(HDC hdc);
private:
	ObjectPos _start;
	ObjectPos _end;
	HoldState _state;
};

