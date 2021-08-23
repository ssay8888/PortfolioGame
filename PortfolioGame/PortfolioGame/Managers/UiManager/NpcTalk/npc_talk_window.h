#pragma once
#include "../Base/base_window.h"


class Npc;
class UiButton;

class NpcTalkWindow :
	public BaseWindow
{
public:
	enum class NpcTalkButton { kClose, kNext, kNo, kOk, kQNo, kYes, kQYes, };
	NpcTalkWindow();
	~NpcTalkWindow();

	void SetNpc(Npc*);
	Npc* GetNpc() const;
	void SetTalkType(ObjectType::NpcTalkType type);
	ObjectType::NpcTalkType GetTalkType() const;

	void AddSayNumber(int32_t value);
	void AddSuccessNumber(int32_t value);

	bool CheckQuest();
	
public:
	static void NextNpcTalk();
	static void AcceptNpcTalk();
	static void CancelNpcTalk();
	static void OkNpcTalk();
private:
	void ReadyWindow() override;
	void UpdateWindow() override;
	void RenderWinodw(HDC hdc) override;

private:
	std::map<NpcTalkButton, std::shared_ptr<UiButton>> _list_button;
	Npc* _this_npc;
	ObjectType::NpcTalkType _talk_type;

	int32_t _say_number;
	int32_t _success_number;
};

