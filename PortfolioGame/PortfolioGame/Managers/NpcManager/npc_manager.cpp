#include "../../pch.h"
#include "npc_manager.h"
#include "../../Utility/xml_reader.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"

void NpcManager::InsertNpc(std::string npc_path, std::shared_ptr<Npc> npc)
{
	_npcs.insert(std::make_pair(npc_path, npc));
}

std::shared_ptr<Npc> NpcManager::FindNpc(std::string npc_path)
{
	auto data = _npcs.find(npc_path);
	if (data != _npcs.end())
	{
		return data->second;
	}
	return nullptr;
}

void NpcManager::LoadNpc()
{
	XmlReader::GetInstance().LoadNpc();
	QuestAlarmImage();
}

std::vector<std::shared_ptr<MyBitmap>>& NpcManager::GetAcceptable()
{
	return _acceptable;
}

std::vector<std::shared_ptr<MyBitmap>>& NpcManager::GetProceeding()
{
	return _proceeding;
}

std::vector<std::shared_ptr<MyBitmap>>& NpcManager::GetCompleted()
{
	return _completed;
}

void NpcManager::SetQuestIconTick()
{
	_quest_icon_tick = GetTickCount64();
}

uint64_t NpcManager::GetQuestIconTick() const
{
	return _quest_icon_tick;
}

void NpcManager::GainQuestFrameNumber(uint32_t value)
{
	_quest_frame_number += value;
}

uint32_t NpcManager::GetQuestFrameNumber() const
{
	return _quest_frame_number;
}

void NpcManager::QuestAlarmImage()
{
	for (int i = 0; i < 8; ++i)
	{
		wchar_t img_path[255];
		swprintf_s(img_path, 255, L"Client\\Ui\\UtilDlgEx.img\\QuestIcon\\QuestIcon.0.%d.bmp", i);
		auto image = std::make_shared<MyBitmap>(MyBitmap());;
		image->Insert_Bitmap(_hWnd, img_path);
		_acceptable.emplace_back(image);
	}
	for (int i = 0; i < 4; ++i)
	{
		wchar_t img_path[255];
		swprintf_s(img_path, 255, L"Client\\Ui\\UtilDlgEx.img\\QuestIcon\\QuestIcon.1.%d.bmp", i);
		auto image = std::make_shared<MyBitmap>(MyBitmap());;
		image->Insert_Bitmap(_hWnd, img_path);
		_proceeding.emplace_back(image);
	}
	for (int i = 0; i < 8; ++i)
	{
		wchar_t img_path[255];
		swprintf_s(img_path, 255, L"Client\\Ui\\UtilDlgEx.img\\QuestIcon\\QuestIcon.2.%d.bmp", i);
		auto image = std::make_shared<MyBitmap>(MyBitmap());;
		image->Insert_Bitmap(_hWnd, img_path);
		_completed.emplace_back(image);
	}
}
