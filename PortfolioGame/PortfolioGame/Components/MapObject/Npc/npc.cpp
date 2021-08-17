#include "../../../pch.h"
#include "npc.h"

#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Managers/ScrollManager/scroll_manager.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../../Managers/MapManager/map_manager.h"
#include "../../../Managers/QuestManager/quest_manager.h"
#include "../../../Managers/NpcManager/npc_manager.h"
#include "../../../Managers/QuestManager/Quest/quest_info.h"
#include "../../../Managers/QuestManager/Quest/SubInfo/quest_check.h"
#include "../Player/Inventory/inventory.h"

Npc::Npc() : GameObject(0),
             _npc_id(0),
             _origin(),
             _frame_tick(0),
             _frame_number(0)
{
}

Npc::~Npc()
{
}

void Npc::InsertImage(std::shared_ptr<MyBitmap> image)
{
	_stand.emplace_back(image);
}

std::vector<std::shared_ptr<MyBitmap>>& Npc::GetStandImages()
{
	return _stand;
}

void Npc::SetOrigin(POINT point)
{
	_origin = point;
}

void Npc::SetOriginX(int x)
{
	_origin.x = x;
}

void Npc::SetOriginY(int y)
{
	_origin.y = y;
}

void Npc::SetNpcId(int32_t npc_id)
{
	_npc_id = npc_id;
}

void Npc::SetNpcPath(std::string path)
{
	_npc_path = path;
}

std::string Npc::GetNpcPath() const
{
	return _npc_path;
}

POINT Npc::GetOrigin() const
{
	return _origin;
}

int32_t Npc::GetNpcId() const
{
	return _npc_id;
}

int Npc::ReadyGameObject()
{
	return 0;
}

void Npc::UpdateGameObject(const float deltaTime)
{
	if (!_stand.empty())
	{
		auto image = _stand[_frame_number % _stand.size()];
		if (image != nullptr)
		{
			_info.cx = image->GetWidth();
			_info.cy = image->GetHeight();
		}
	}
	
}

void Npc::RenderGameObject(HDC hdc)
{
	UpdateRectGameObject();
	if (!_stand.empty())
	{
		//Rectangle(hdc,
		//	_rect.left + static_cast<int>(ScrollManager::GetScrollX()),
		//	_rect.top + static_cast<int>(ScrollManager::GetScrollY()),
		//	_rect.right + static_cast<int>(ScrollManager::GetScrollX()),
		//	_rect.bottom + static_cast<int>(ScrollManager::GetScrollY()));
		_stand[_frame_number % _stand.size()]->RenderBitmapImage(hdc, 
			static_cast<int>(_info.x - (_info.cx >> 1) + ScrollManager::GetScrollX()), 
			static_cast<int>(_info.y - (_info.cy * 0.5) + ScrollManager::GetScrollY()),
			_info.cx,
			_info.cy);
		QuestCheckRender(hdc);
	}
}

void Npc::LateUpdateGameObject()
{
	if (GetTickCount64() > _frame_tick + 300)
	{
		_frame_tick = GetTickCount64();
	}
}

void Npc::QuestCheckRender(HDC hdc)
{
	auto player = MapManager::GetInstance()->GetPlayer();
	auto quest_info = QuestManager::GetInstance()->FindQuestInfo(this->GetNpcId());
	if (quest_info != nullptr)
	{
		auto clear_quest = player->FindClearQuest(this->GetNpcId());
		if (clear_quest != nullptr)
		{
			return;
		}
		auto npc_manager = NpcManager::GetInstance();
		auto ing_quest = player->FindIngQuest(this->GetNpcId());
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
					auto image = npc_manager->GetCompleted()[npc_manager->GetQuestFrameNumber() % npc_manager->GetCompleted().size()];
					if (image != nullptr)
					{
						image->RenderBitmapImage(hdc,
							static_cast<int>(_info.x + ScrollManager::GetScrollX()),
							static_cast<int>(_info.y - (_info.cy >> 1) - image->GetHeight() + ScrollManager::GetScrollY()),
							image->GetWidth(),
							image->GetHeight());
					}
					return;
				}
			}
			auto image = npc_manager->GetProceeding()[npc_manager->GetQuestFrameNumber() % npc_manager->GetProceeding().size()];
			if (image != nullptr)
			{
				image->RenderBitmapImage(hdc,
					static_cast<int>(_info.x + ScrollManager::GetScrollX()),
					static_cast<int>(_info.y - (_info.cy >> 1) - image->GetHeight() + ScrollManager::GetScrollY()),
					image->GetWidth(),
					image->GetHeight());
			}
			return;
		}
		auto image = npc_manager->GetAcceptable()[npc_manager->GetQuestFrameNumber() % npc_manager->GetAcceptable().size()];
		if (image != nullptr)
		{
			image->RenderBitmapImage(hdc,
				static_cast<int>(_info.x  + ScrollManager::GetScrollX()) ,
				static_cast<int>(_info.y - (_info.cy >> 1) - image->GetHeight() + ScrollManager::GetScrollY()) ,
				image->GetWidth(), 
				image->GetHeight());
		}
	}
}
