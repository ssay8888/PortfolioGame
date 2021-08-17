#pragma once
#include "../Base/base_window.h"

namespace ObjectType
{
	enum class NpcTalkType;
}

class Npc;
class UiButton;

class NpcTalkWindow :
	public BaseWindow
{
public:
	enum class NpcTalkButton { kClose, kNext, kNo, kOk, kQNo, kYes, kQYes, };
	NpcTalkWindow();
	~NpcTalkWindow();

public:
	static void CancelNpcTalk();
private:
	void ReadyWindow() override;
	void UpdateWindow() override;
	void RenderWinodw(HDC hdc) override;

private:
	std::map<NpcTalkButton, std::shared_ptr<UiButton>> _list_button;
	std::shared_ptr<Npc> _this_npc;
	ObjectType::NpcTalkType _talk_type;
};

