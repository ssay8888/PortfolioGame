#include "../../../pch.h"
#include "npc_talk_window.h"

#include "../ui_button.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Components/MapObject/Item/item.h"
#include "../../UiManager/ui_manager.h"
#include "../../../Components/MapObject/Npc/npc.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../KeyManaer/key_manager.h"
#include "../../QuestManager/quest_manager.h"
#include "../../QuestManager/Quest/quest_info.h"
#include "../../QuestManager/Quest/SubInfo/quest_say.h"
#include "../../QuestManager/Quest/SubInfo/quest_check.h"
#include "../../MapManager/map_manager.h"
#include "../../../Components/MapObject/Player/Inventory/inventory.h"
#include "../../ItemManager/item_manager.h"
#include "../../QuestManager/Quest/SubInfo/quest_reward.h"
#include "../../Skins/skin_manager.h"
#include "../../../Components/MapObject/Player/Inventory/eqp_inventory.h"
#include "../../Skins/skin_info.h"

NpcTalkWindow::NpcTalkWindow(): _this_npc(nullptr),
                                _talk_type(),
                                _say_number(0),
                                _success_number(0)
{
}

NpcTalkWindow::~NpcTalkWindow()
{
}

void NpcTalkWindow::SetNpc(Npc* npc)
{
	_this_npc = npc;
	_say_number = 0;
	_success_number = 0;
}

Npc* NpcTalkWindow::GetNpc() const
{
	return _this_npc;
}

void NpcTalkWindow::SetTalkType(ObjectType::NpcTalkType type)
{
	_talk_type = type;
}

ObjectType::NpcTalkType NpcTalkWindow::GetTalkType() const
{
	return _talk_type;
}

void NpcTalkWindow::AddSayNumber(int32_t value)
{
	_say_number += value;
}

void NpcTalkWindow::AddSuccessNumber(int32_t value)
{
	_success_number += value;
}

void NpcTalkWindow::NextNpcTalk()
{
	UiManager::GetInstance()->GetNpcTalkWindow()->AddSayNumber(1);
}

void NpcTalkWindow::AcceptNpcTalk()
{
	auto player = MapManager::GetInstance()->GetPlayer();
	auto npc = UiManager::GetInstance()->GetNpcTalkWindow()->GetNpc();
	auto quest_info = QuestManager::GetInstance()->FindQuestInfo(npc->GetNpcId());
	if (quest_info != nullptr)
	{
		player->InsertIngQuest(quest_info);
	}
	UiManager::GetInstance()->GetNpcTalkWindow()->SetShow(false);
}

void NpcTalkWindow::CancelNpcTalk()
{
	UiManager::GetInstance()->GetNpcTalkWindow()->SetShow(false);
}

void NpcTalkWindow::OkNpcTalk()
{
	auto player = MapManager::GetInstance()->GetPlayer();
	auto npc_talk_window = UiManager::GetInstance()->GetNpcTalkWindow();
	auto npc = npc_talk_window->GetNpc();
	auto quest_info = QuestManager::GetInstance()->FindQuestInfo(npc->GetNpcId());
	if (npc_talk_window->CheckQuest())
	{
		if (quest_info != nullptr)
		{
			player->RemoveIngQuest(quest_info);
			player->InsertClearQuest(quest_info);
		}

		auto quest_check = quest_info->GetCheck();
		if (!quest_check.empty())
		{
			bool isCmpleted = true;
			for (auto check : quest_check)
			{
				auto inven = player->GetInventory(::ObjectType::InventoryTabState::kEtc);
				auto total_item_quantity = inven->TotalQuantity(check->GetItemId());
				if (total_item_quantity > check->GetPrice())
				{
					int price = check->GetPrice();
					for (int i = 0; i < 96; ++i)
					{
						auto item = inven->GetItem()[i];
						if (item != nullptr)
						{
							if (item->GetItemId() == check->GetItemId())
							{
								if (item->GetQuantity() >= price)
								{
									item->GainQuantity(-price);
									price = 0;
								}
								else
								{
									price -= item->GetQuantity();
									item->SetQuantity(0);
								}
							}
						}
						if (price <= 0)
						{
							break;
						}
					}
				}
			}
		}
		for(auto reward : quest_info->GetReward())
		{
			auto inven = player->GetInventory(ObjectType::InventoryTabState::kConsume);
			if (!strcmp(reward->GetRewardType().c_str(), "exp"))
			{
				player->GainExp(reward->GetValue());
			}
			else if (!strcmp(reward->GetRewardType().c_str(), "meso"))
			{
				player->GainMeso(reward->GetValue());
			}
			else if (!strcmp(reward->GetRewardType().c_str(), "item"))
			{
				if (reward->GetItemId() >= 2000000)
				{
					auto item = ItemManager::GetInstance()->FindCopyItem(reward->GetItemId());
					item->SetQuantity(reward->GetValue());
					inven->AddItem(inven->FindFreeSlot(), item);
				}
				else
				{

					auto item = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(reward->GetItemId()));
					player->GetEqpInventory()->AddItem(player->GetEqpInventory()->FindFreeSlot(), std::make_shared<SkinInfo>(SkinInfo(*item)));
				}
			}
		}
	}
	UiManager::GetInstance()->GetNpcTalkWindow()->SetShow(false);
}

void NpcTalkWindow::ReadyWindow()
{
	_background = std::make_shared<MyBitmap>(MyBitmap());
	_background->Insert_Bitmap(_hWnd, L"Client\\Ui\\UtilDlgEx.img\\background.bmp");
	_info.x = WindowCX / 2 - _background->GetWidth() / 2.f;
	_info.y = WindowCY / 2 - _background->GetHeight() / 2.f;
	_info.cx = _background->GetWidth();
	_info.cy = _background->GetHeight();
	_key_manager = new KeyManager();

	auto button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 8, _info.y + 179});
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtClose");
	button->SetCallBack(CancelNpcTalk);
	_list_button.insert(std::make_pair(NpcTalkButton::kClose, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 449, _info.y + 127 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtNext");
	button->SetCallBack(NextNpcTalk);
	_list_button.insert(std::make_pair(NpcTalkButton::kNext, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 461, _info.y + 177 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtNo");
	button->SetCallBack(CancelNpcTalk);
	_list_button.insert(std::make_pair(NpcTalkButton::kNo, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 472, _info.y + 178 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtOK");
	button->SetCallBack(OkNpcTalk);
	_list_button.insert(std::make_pair(NpcTalkButton::kOk, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 461, _info.y + 177 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtQNo");
	button->SetCallBack(CancelNpcTalk);
	_list_button.insert(std::make_pair(NpcTalkButton::kQNo, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 396, _info.y + 177 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtYes");
	button->SetCallBack(AcceptNpcTalk);
	_list_button.insert(std::make_pair(NpcTalkButton::kYes, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 396, _info.y + 177 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtQYes");
	button->SetCallBack(AcceptNpcTalk);
	_list_button.insert(std::make_pair(NpcTalkButton::kQYes, button));


}

void NpcTalkWindow::UpdateWindow()
{
	_key_manager->KeyUpdate();
	for (auto button : _list_button)
	{
		if (_show)
		{
			button.second->UpdateButton();
		}
	}
	_talk_type = ObjectType::NpcTalkType::kOk;
}

void NpcTalkWindow::RenderWinodw(HDC hdc)
{
	if (!IsShow())
	{
		return;
	}

	auto player = MapManager::GetInstance()->GetPlayer();
	std::shared_ptr<QuestInfo> quest_info = nullptr;
	std::wstring str;
	if (_this_npc != nullptr)
	{
		quest_info = QuestManager::GetInstance()->FindQuestInfo(_this_npc->GetNpcId());
		if (player->FindClearQuest(quest_info->GetNpcId()) != nullptr)
		{
			SetShow(false);
			return;
		}
		_background->RenderBitmapImage(hdc,
			static_cast<int>(_info.x),
			static_cast<int>(_info.y),
			_background->GetWidth(),
			_background->GetHeight());

		if (quest_info != nullptr)
		{
			auto ing_quest = player->FindIngQuest(_this_npc->GetNpcId());
			if (CheckQuest())
			{
				if (ing_quest != nullptr)
				{
					auto say = ing_quest->GetSuccess()[_say_number];
					_talk_type = say->GetType();
					str.append(StringTools::StringToWString(say->GetSay().c_str()));
				}
			}
			else
			{
				if (ing_quest != nullptr)
				{
					auto say = ing_quest->GetIngSay()[_say_number];
					_talk_type = say->GetType();
					str.append(StringTools::StringToWString(say->GetSay().c_str()));
				}
				else if (quest_info->GetSay().size() > _say_number)
				{
					auto say = quest_info->GetSay()[_say_number];
					_talk_type = say->GetType();
					str.append(StringTools::StringToWString(say->GetSay().c_str()));
				}
			}
		}
		auto images = _this_npc->GetStandImages();
		if (!images.empty())
		{
			images[0]->RenderBitmapImage(hdc,
				static_cast<int>(_info.x) + 68 - (images[0]->GetWidth() >> 1),
				static_cast<int>(_info.y) + 106 - images[0]->GetHeight(),
				images[0]->GetWidth(),
				images[0]->GetHeight());
		}
	}
	auto close_button_pair = _list_button.find(NpcTalkButton::kClose);
	if (close_button_pair != _list_button.end())
	{
		auto close_button = close_button_pair->second;
		close_button->RenderButtonUi(hdc);
	}
	switch (_talk_type) {
	case ObjectType::NpcTalkType::kNext:
	{
		for (auto& button : _list_button)
		{
			if (button.first == NpcTalkButton::kNext)
			{
				auto next_button = button.second;
				next_button->RenderButtonUi(hdc);
				if (quest_info != nullptr)
				{
					RECT rc;
					rc.left = static_cast<int>(_info.x) + 153;
					rc.top = static_cast<int>(_info.y) + 27;
					rc.right = rc.left + 348;
					rc.bottom = rc.top + 120;
					StringTools::CreateDrawText(hdc, rc, str, 11, RGB(0, 0, 0));
					/*StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 153, static_cast<int>(_info.y) + 27,
						str, 11, RGB(0, 0, 0));*/
				}
				button.second->SetState(UiButton::ButtonState::kNormal);
			}
			else if (button.first != NpcTalkButton::kClose)
			{
				button.second->SetState(UiButton::ButtonState::kDisable);
			}
		}
		break;
	}
	case ObjectType::NpcTalkType::kOk: 
	{
		for (auto& button : _list_button)
		{
			if (button.first == NpcTalkButton::kOk)
			{
				auto next_button = button.second;
				next_button->RenderButtonUi(hdc);
				if (quest_info != nullptr)
				{
					RECT rc;
					rc.left = static_cast<int>(_info.x) + 153;
					rc.top = static_cast<int>(_info.y) + 27;
					rc.right = rc.left + 348;
					rc.bottom = rc.top + 120;
					StringTools::CreateDrawText(hdc, rc, str, 11, RGB(0, 0, 0));
				}
				button.second->SetState(UiButton::ButtonState::kNormal);
			}
			else if (button.first != NpcTalkButton::kClose)
			{
				button.second->SetState(UiButton::ButtonState::kDisable);
			}
		}
		break;
	}
	case ObjectType::NpcTalkType::kQYesNo: 
	{

		for (auto& button : _list_button)
		{
			if (button.first == NpcTalkButton::kQYes || button.first == NpcTalkButton::kQNo)
			{
				auto next_button = button.second;
				next_button->RenderButtonUi(hdc);
				if (quest_info != nullptr)
				{
					RECT rc;
					rc.left = static_cast<int>(_info.x) + 153;
					rc.top = static_cast<int>(_info.y) + 27;
					rc.right = rc.left + 348;
					rc.bottom = rc.top + 120;
					StringTools::CreateDrawText(hdc, rc, str, 11, RGB(0, 0, 0));
				}
				button.second->SetState(UiButton::ButtonState::kNormal);
			}
			else if (button.first != NpcTalkButton::kClose)
			{
				button.second->SetState(UiButton::ButtonState::kDisable);
			}
		}
		break;
	}
	default:;
	}
}

bool NpcTalkWindow::CheckQuest()
{
	auto player = MapManager::GetInstance()->GetPlayer();
	auto quest_info = QuestManager::GetInstance()->FindQuestInfo(_this_npc->GetNpcId());
	if (quest_info != nullptr)
	{
		auto clear_quest = player->FindClearQuest(_this_npc->GetNpcId());
		if (clear_quest != nullptr)
		{
			return false;
		}
		auto ing_quest = player->FindIngQuest(_this_npc->GetNpcId());
		if (ing_quest != nullptr)
		{
			auto quest_check = quest_info->GetCheck();
			if (!quest_check.empty())
			{
				bool isCmpleted = true;
				for (auto check : quest_check)
				{
					auto total_item_quantity = player->GetInventory(::ObjectType::InventoryTabState::kEtc)->TotalQuantity(check->GetItemId());
					if (check->GetPrice() > total_item_quantity)
					{
						isCmpleted = false;
						break;
					}
				}
				if (isCmpleted)
				{
					return true;
				}
			}
		}
	}
	return false;
}
