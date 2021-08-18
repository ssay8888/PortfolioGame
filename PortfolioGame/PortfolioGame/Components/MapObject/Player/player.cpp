#include "../../../pch.h"
#include "player.h"
#include "../foot_hold.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../../Common/Managers/CollisionManager/Collision_Manager.h"
#include "../../../Components/MapObject/Monster/monster.h"
#include "../../../Managers/EffectManager/effect_manager.h"
#include "../../../Managers/ItemManager/item_manager.h"
#include "../../../Managers/KeyManaer/key_manager.h"
#include "../../../Managers/MapManager/map_manager.h"
#include "../../../Managers/MapManager/Map/map_instance.h"
#include "../../../Managers/ScrollManager/scroll_manager.h"
#include "../../../Managers/SkillManager/Skill/skill.h"
#include "../../../Managers/SkillManager/Skill/skill_info.h"
#include "../../../Managers/Skins/skin_frame.h"
#include "../../../Managers/Skins/skin_info.h"
#include "../../../Managers/Skins/skin_item.h"
#include "../../../Managers/Skins/skin_manager.h"
#include "../../../Managers/Skins/skin_parts.h"
#include "../../../Managers/UiManager/ui_manager.h"
#include "../../../Managers/UiManager/Inventory/inventory_window.h"
#include "../../../Managers/UiManager/QuickSlot/quick_slot.h"
#include "../../../Managers/UiManager/NpcTalk/npc_talk_window.h"
#include "../../../Managers/QuestManager/quest_manager.h"
#include "../Npc/npc.h"

#include "../../../Managers/ScenManager/InGameScene/in_game_scene.h"

#include "../../MapObject/portal.h"
#include "../Item/item.h"
#include "Damage/damage_handler.h"
#include "Equip/equipment.h"
#include "Inventory/eqp_inventory.h"
#include "Inventory/inventory.h"
#include "MagicAttack/magic_attack.h"


#include <time.h>

#include "../../game_mouse.h"
#include "../../../Managers/QuestManager/Quest/quest_info.h"
#include "../../../Managers/ShopManager/shop_manager.h"
#include "../../../Managers/UiManager/Shop/shop_window.h"


Player::Player(uint8_t layer) :
                              GameObject(layer),
                              _this_frame_max_count(0),
                              _frame_revers(false),
                              _frame_state("stand1"),
                              _frame_tick(0),
                              _player_info({51, 1000, 0, 432, 
								  1000, 0, 534, 100, 100, 1000, 222,
                              	13, 4, 4, 4,
                              	0, 0, 0, 0,
                              	0, 0, 0, 0,
                              	0, 0,0,"무야호"}),
                              _now_foothold(nullptr),
                              _next_foothold(nullptr),
                              _is_first_foothold(false)
{
	Player::ReadyGameObject();
}

Player::~Player()
{
	for (int i = 0; i < ::ObjectType::InventoryTabState::kEnd; ++i)
	{
		delete _inventory[i];
	}
	delete _eqp_inventory;
	delete _equipment;
	SelectObject(_memDC, _old_bitmap);
	DeleteObject(_bitmap);
	DeleteDC(_memDC);
}

int Player::ReadyGameObject()
{
	_info.x = 100.f;
	_info.y = 100.f;
	_info.cx = 42;
	_info.cy = 64;
	_speed = 5.f;
	LoadCharacterFrame("stand1");

	HDC hDC = GetDC(_hWnd);
	ScrollManager::SetScrollX(1024/2);
	ScrollManager::SetScrollY(768/2);
	_damage_handler = new DamageHandler();
	_memDC = CreateCompatibleDC(hDC);
	_bitmap = CreateCompatibleBitmap(hDC, 1024, 768);
	_old_bitmap = (HBITMAP)SelectObject(_memDC, _bitmap);
	ReleaseDC(_hWnd, hDC);

	srand(int(time(NULL)));
	for (int i = 0; i < ::ObjectType::InventoryTabState::kEnd; ++i)
	{
		_inventory[i] = new Inventory();
	}
	_eqp_inventory = new EqpInventory();
	_equipment = new Equipment();
	RecalcEqpStat();
    return 0;
}

void Player::UpdateGameObject(const float deltaTime)
{
	auto keymanager = KeyManager::GetInstance();
	float totalMoveX = 0;
	float totalMoveY = 0;
	float outY = 0;
	FootHold* tempHold;
	bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &tempHold);
	const bool finished_attack_frame = (_attack_skill != nullptr ? _attack_skill->IsFinishedFrame() : true);
	if (finished_attack_frame && !_is_fly && !_is_jump && !_is_prone && keymanager->KeyPressing(KEY_DOWN))
	{
		float outX;
		FootHold* outHold = nullptr;
		bool isRope = MapManager::GetInstance()->LadderRopeCollsition(this, &outX, &outHold);
		if (isRope)
		{
			if (!_is_rope)
			{
				switch (outHold->GetState())
				{
				case FootHold::HoldState::kLadder:
					this->ChangeFrameState("ladder");
					break;
				case FootHold::HoldState::kRope:
					this->ChangeFrameState("rope");
					break;
				default:
					break;
				}
				_is_rope = true;
				_info.x = outX;
			}
			_info.y += GetSpeed();
			if (CollisionManager::LineAndRectCollsition(MapManager::GetInstance()->GetNowMap()->GetFootHoldList(), this))
			{
				if (!_is_first_foothold)
				{
					_info.y -= GetSpeed();
				}
				else if (_is_first_foothold && _next_foothold == nullptr)
				{
					_info.y -= GetSpeed();
					_is_rope = false;
				}
			}
			else if (_info.y >= outY)
			{
				_info.y = outY;
				_is_rope = false;
			}
			else
			{
				_info.y -= GetSpeed();
				totalMoveY += GetSpeed();
			}
		}
		else
		{
			_is_rope = false;
		}
	}

	if (_info.y >= outY)
	{
		if (finished_attack_frame && !_is_attacking && !_is_jump && !_is_prone && !_is_rope && keymanager->KeyPressing(KEY_DOWN))
		{
			this->ChangeFrameState("prone");
			_is_prone = true;
		}
	}

	if (finished_attack_frame && keymanager->KeyUp(KEY_DOWN))
	{
		if (!_is_attacking && !_is_rope)
		{
			this->ChangeFrameState("stand1");
			_is_prone = false;
		}
	}
	if (finished_attack_frame && !_is_prone)
	{
		if (keymanager->KeyPressing(KEY_LEFT))
		{
			if (_is_rope)
			{
				if (keymanager->KeyPressing(KEY_C))
				{
					_is_rope = false;
					this->ChangeFrameState("jump");
					_is_jump = true;
					totalMoveX -= GetSpeed();
				}
			}
			else
			{
				if (((_is_jump || _is_fly) && _is_attacking) || !_is_attacking)
				{
					totalMoveX -= GetSpeed();
				}
			}
		}
		if (keymanager->KeyPressing(KEY_RIGHT))
		{
			if (_is_rope)
			{
				if (keymanager->KeyPressing(KEY_C))
				{
					_is_rope = false;
					this->ChangeFrameState("jump");
					_is_jump = true;
					totalMoveX += GetSpeed();
				}
			}
			else
			{
				if (((_is_jump || _is_fly) && _is_attacking) || !_is_attacking)
				{
					totalMoveX += GetSpeed();
				}
			}
		}
		if (_info.y >= outY)
		{
			if (!_is_attacking && !_is_jump && keymanager->KeyPressing(KEY_C))
			{
				this->ChangeFrameState("jump");
				_is_jump = true;
			}
		}
	}


	if (finished_attack_frame && !_is_prone && keymanager->KeyPressing(KEY_UP))
	{
		auto rope = MapManager::GetInstance()->GetNowMap()->GetRopeLadderList();
		float outX = 0;
		FootHold* outHold = nullptr;
		bool isRope = MapManager::GetInstance()->LadderRopeCollsition(this, &outX, &outHold);


		if (isRope)
		{
			_info.x = outX;
			totalMoveY -= GetSpeed();
			if (!_is_rope)
			{
				switch (outHold->GetState())
				{
				case FootHold::HoldState::kLadder:
					this->ChangeFrameState("ladder");
					break;
				case FootHold::HoldState::kRope:
					this->ChangeFrameState("rope");
					break;
				default:
					break;
				}
				_is_rope = true;
				_is_fly = false;
			}
		}
		else 
		{
			Portal* outPortal = nullptr;
			
			if (MapManager::GetInstance()->PortalCollsition(this, &outPortal))
			{
				if (GetTickCount64() > _portal_tick + 1000)
				{
					MapManager::GetInstance()->ChangeMap(outPortal->GetNextMap(), outPortal->GetNextMapSpawnPos());
					_portal_tick = GetTickCount64();
				}
			}
			_is_rope = false;
		}
	}
	if (!_is_rope &&  !_is_prone && finished_attack_frame)
	{
		ApplySkill();
	}
	TakeDamage();
	bool isKnockback = GetTickCount64() < _knockback_tick + _knockback_time;
	int32_t knockbackMoveX = 0;
	int32_t knockbackMoveY = 0;
	if (isKnockback)
	{
		if (_knockback_facing_direction)
		{
			knockbackMoveX += 3;
			knockbackMoveY -= 2;
			totalMoveX += knockbackMoveX;
			totalMoveY += knockbackMoveY;
		}
		else
		{
			knockbackMoveX -= 3;
			knockbackMoveY -= 2;
			totalMoveX += knockbackMoveX;
			totalMoveY += knockbackMoveY;
		}
	}
	else
	{
		_knockback_tick = 0;
	}

	if (totalMoveX != 0 || totalMoveY != 0)
	{
		if (totalMoveX < 0 && knockbackMoveX == 0)
		{
			this->SetFacingDirection(0);
		}
		else if (totalMoveX > 0 && knockbackMoveX == 0)
		{
			this->SetFacingDirection(1);
		}
		if (!_is_attacking && !_is_fly && !_is_rope && !_is_jump && strcmp(GetFrameState(), "walk1"))
		{
			this->ChangeFrameState("walk1");
		}

		float outY = 0;
		_info.x += totalMoveX;
		_info.y += totalMoveY;
		UpdateRectGameObject();
		if (MapManager::GetInstance()->FootholdAndRectCollision(this))
		{
			_info.x -= totalMoveX;
			// y좌표는 가도됨.
		}
	}
	else
	{
		if (!_is_attacking && !_is_fly && !_is_rope && !_is_jump && !_is_prone)
		{
			if (IsAlertStateTick())
			{
				this->ChangeFrameState("alert");
			}
			else
			{
				this->ChangeFrameState("stand1");
			}
		}
	}


	if (!_is_attacking && !_is_rope && keymanager->KeyPressing(KEY_A))
	{
		if (_is_prone) 
		{
			this->ChangeFrameState("proneStab");
		}
		else
		{
			this->ChangeFrameState("swingO2");
		}
		Player::TryMeleeAttack();
		UpdateAlertTick();
	}
	if (keymanager->KeyPressing(KEY_B))
	{
		this->ChangeFrameState("swingT1");
	}

	if (keymanager->KeyPressing(KEY_Z))
	{
		auto& drop_list = MapManager::GetInstance()->GetNowMap()->GetListDropItem();
		for (auto drop = drop_list.begin(); drop != drop_list.end(); )
		{
			RECT rc;
			auto icon = drop->second->GetIconRaw();
			int left = drop->first.x;
			int top = drop->first.y - icon->GetHeight();
			int right = left + icon->GetWidth();
			int bottom = top + icon->GetHeight();
			RECT item_rect{
			left,
			top,
				right,
			bottom,
			};

			if (IntersectRect(&rc, &item_rect, &_rect))
			{
				const auto inven_type = InventoryWindow::SearchItemTab(drop->second->GetItemId());

				if (drop->second->GetItemId() < 9000000)
				{
					if (inven_type != ::ObjectType::kEnd)
					{
						_inventory[inven_type]->AddItem(_inventory[inven_type]->FindFreeSlot(), drop->second);
						drop = drop_list.erase(drop);
						continue;;
					}
				}
				else if (drop->second->GetItemId() < 9000005)
				{
					GainMeso(drop->second->GetQuantity());
					drop = drop_list.erase(drop);
					continue;;
				}
			}
			++drop;
		}
	}
	if (keymanager->KeyDown(KEY_LBUTTON))
	{
		auto pos = InGameScene::GetMouse()->GetPoint();
		auto npc_list = MapManager::GetInstance()->NpcCollision(pos);
		
		if (!npc_list.empty())
		{
			auto npc_talk = UiManager::GetInstance()->GetNpcTalkWindow();
			auto quest_info = QuestManager::GetInstance()->FindQuestInfo((*npc_list.begin())->GetNpcId());
			if (quest_info != nullptr)
			{
				npc_talk->SetShow(true);
				npc_talk->SetNpc((*npc_list.begin()));
				_select_npc = quest_info;
			}
			auto shop = ShopManager::GetInstance()->FindShop((*npc_list.begin())->GetNpcId());
			if (!shop.empty())
			{
				auto shop_window = UiManager::GetInstance()->GetShopWindow();
				shop_window->SetShow(true);
				shop_window->LoadShopData((*npc_list.begin())->GetNpcId());
			}
		}

	}
	if (keymanager->KeyPressing(KEY_F1))
	{
		for (int i = 0; i < 24; ++i)
		{
			auto item = ItemManager::GetInstance()->FindCopyItem(2000000 + i);
			item->SetQuantity(rand() % 100);
			_inventory[::ObjectType::kConsume]->AddItem(i, item);
		}
		for (int i = 0; i < 3; ++i)
		{
			auto item = ItemManager::GetInstance()->FindCopyItem(2001000 + i);
			item->SetQuantity(rand() % 100);
			_inventory[::ObjectType::kConsume]->AddItem(i + 24, item);
		}
		for (int i = 0; i < 25; ++i)
		{
			auto item = ItemManager::GetInstance()->FindCopyItem(2001500 + i);
			item->SetQuantity(rand() % 100);
			_inventory[::ObjectType::kConsume]->AddItem(i + 24 + 3, item);
		}
		for (int i = 0; i < 12; ++i)
		{
			auto item = ItemManager::GetInstance()->FindCopyItem(2002000 + i);
			item->SetQuantity(rand() % 100);
			_inventory[::ObjectType::kConsume]->AddItem(i + 24 + 3 + 25, item);
		}

		for (int i = 0; i < 96; ++i)
		{
			auto item = ItemManager::GetInstance()->FindCopyItem(4000000 + i);
			item->SetQuantity(rand() % 100);
			_inventory[::ObjectType::kEtc]->AddItem(i, item);
		}
		auto shoesParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1072001));
		_eqp_inventory->AddItem(0, std::make_shared<SkinInfo>(SkinInfo(*shoesParts)));
		auto itemParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1302000));
		_eqp_inventory->AddItem(1, std::make_shared<SkinInfo>(SkinInfo(*itemParts)));
		auto coatParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1040002));
		_eqp_inventory->AddItem(2, std::make_shared<SkinInfo>(SkinInfo(*coatParts)));
		auto coatParts2 = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1040000));
		_eqp_inventory->AddItem(3, std::make_shared<SkinInfo>(SkinInfo(*coatParts2)));
		auto pantsParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1060002));
		_eqp_inventory->AddItem(4, std::make_shared<SkinInfo>(SkinInfo(*pantsParts)));
		auto pantsParts2 = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1060000));
		_eqp_inventory->AddItem(5, std::make_shared<SkinInfo>(SkinInfo(*pantsParts2)));
		pantsParts2 = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1072168));
		_eqp_inventory->AddItem(6, std::make_shared<SkinInfo>(SkinInfo(*pantsParts2)));
		for (int i = 0; i < 6; i++)
		{
			pantsParts2 = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(1382000 + i));
			_eqp_inventory->AddItem(7 + i, std::make_shared<SkinInfo>(SkinInfo(*pantsParts2)));
		}
		auto item = ItemManager::GetInstance()->FindCopyItem(9000000);
		POINT ps = { static_cast<int>(_info.x), static_cast<int>(_rect.bottom) };
		auto data = std::make_pair(ps, item);
		MapManager::GetInstance()->GetNowMap()->AddDropItem(data);
	}

	if (_frame_this != nullptr)
	{
		if (_is_rope)
		{
			uint64_t tick = GetTickCount64();
			if (tick > _frame_tick + _frame_this->GetDelay() && totalMoveY != 0)
			{
				_frame_nummber++;
				_frame_tick = tick;
			}
		}
	}
	ScrollMove();
	_damage_handler->UpdateDamages();
}

void Player::LoadCharacterFrame(std::string frameName, uint16_t frameCount)
{
	char bodyStr[100];
	char headStr[100];
	uint16_t maxSize = 0;
	//std::vector<SkinItem*> bodyList;
	//std::vector<SkinItem*> headList;
	//for (; maxSize < 100; maxSize++)
	//{
		std::vector<SkinItem*> tempList;
		snprintf(bodyStr, 100, "000%05d.img/%s/%d", _skin_id + 2000, frameName.c_str(), maxSize);
		snprintf(headStr, 100, "000%05d.img/%s/%d", _skin_id + 12000, frameName.c_str(), maxSize);
		auto bodySkinInfo = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(_skin_id + 2000));
		auto headSkinInfo = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(_skin_id + 12000));
		auto hiarParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(30000));
		auto faceParts = SkinManager::GetInstance()->GetBodySkinInfo(std::to_string(20000));
		if (bodySkinInfo != nullptr && headSkinInfo != nullptr)
		{
			_skin_frames.insert({ frameName, bodySkinInfo->FindBodySkinItem(frameName) });
			_head_skin_frames.insert({ frameName, headSkinInfo->FindHeadSkinItem(frameName) });
			//bodyList.push_back(bodySkinInfo->FindBodySkinItem(frameName));
			//headList.push_back(headSkinInfo->FindHeadSkinItem(frameName));
		}
		_skin_frames.erase(std::to_string(30000));
		_skin_frames.insert({ std::to_string(30000), hiarParts->FindBodySkinItem(frameName) });
		_skin_frames.erase(std::to_string(20000));
		_skin_frames.insert({ std::to_string(20000), faceParts->FindBodySkinItem("default") });
	//}
}

void Player::RenderCharacter(HDC hdc)
{
	this->LoadCharacterFrame(_frame_state);

	auto bodyFrameIter = _skin_frames.find(_frame_state.c_str());
	auto headFrameIter = _head_skin_frames.find(_frame_state.c_str());
	auto hairFrameIter = _skin_frames.find(std::to_string(30000));
	auto faceFrameItre = _skin_frames.find(std::to_string(20000));
	if (bodyFrameIter != _skin_frames.end() 
		&& headFrameIter != _head_skin_frames.end())
	{

		std::vector<SkinParts*> partsFrames;
		std::vector<SkinParts*> offsets;
		SkinParts* bodyFrame = nullptr;
		std::vector<std::pair<std::string, ObjectPos>> list;
		_this_frame_max_count = bodyFrameIter->second->GetFrameSize();
		auto bodyskinItem = bodyFrameIter->second;
		auto headskinItem = headFrameIter->second;
		auto bodyHairParts = hairFrameIter->second;
		auto faceParts = faceFrameItre->second;

		if (bodyskinItem && headskinItem)
		{
			auto bodyFrames = bodyskinItem->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
			auto bodyParts = bodyFrames->GetParts();
			for (auto part = bodyParts->begin(); part != bodyParts->end(); ++part)
			{
				for (auto frame = part->second->GetMaps()->begin(); frame != part->second->GetMaps()->end(); ++frame)
				{
					if ((!strcmp(part->second->GetZ().c_str(), "body") || !strcmp(part->second->GetZ().c_str(), "backBody")))
					{
						_frame_this = bodyFrames;
						bodyFrame = part->second;
					}
				}
				partsFrames.emplace_back(part->second);
				offsets.emplace_back(part->second);
			}
			for (auto& data : GetEquipment()->GetEquipItems())
			{
				auto item = data.second->FindBodySkinItem(_frame_state);
				if(item != nullptr)
				{
					auto itemFrames = item->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
					auto itemParts = itemFrames->GetParts();
					for (auto part = itemParts->begin(); part != itemParts->end(); ++part)
					{
						partsFrames.emplace_back(part->second);
						offsets.emplace_back(part->second);
					}
				}
			}
			if (bodyHairParts)
			{
				auto itemFrames = bodyHairParts->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
				auto itemParts = itemFrames->GetParts();
				for (auto part = itemParts->begin(); part != itemParts->end(); ++part)
				{
					partsFrames.emplace_back(part->second);
					offsets.emplace_back(part->second);
				}
			}
			if (faceParts && !_is_rope)
			{
				auto itemFrames = faceParts->FindFrame("face");
				auto itemParts = itemFrames->GetParts();
				for (auto part = itemParts->begin(); part != itemParts->end(); ++part)
				{
					partsFrames.emplace_back(part->second);
					offsets.emplace_back(part->second);
				}
			}
			auto headFrames = headskinItem->FindFrame(std::to_string(_frame_nummber % _this_frame_max_count));
			auto headParts = headFrames->GetParts();
			for (auto part = headParts->begin(); part != headParts->end(); ++part)
			{
				partsFrames.emplace_back(part->second);
			    offsets.emplace_back(part->second);
			}
		}

		std::sort(partsFrames.begin(), partsFrames.end(),
			[](auto& lhs, auto& rhs) {
				return lhs->GetPosition() < rhs->GetPosition();
			});
		std::sort(offsets.begin(), offsets.end(),
			[](auto& lhs, auto& rhs) {
				return lhs->GetPosition() < rhs->GetPosition();
			});

		list.push_back({ "navel", {} });
		if (!strcmp(_frame_state.c_str(), "alert"))
		{
			switch (_frame_nummber % 3)
			{
			case 0:
				list.push_back({ "handMove", {-8, -2} });
				break;
			case 1:
				list.push_back({ "handMove", {-10, 0} });
				break;
			case 2:
				list.push_back({ "handMove", {-12, 3} });
				break;
			}
		}

		for (auto offsetPairing = offsets.begin(); offsetPairing != offsets.end();)
		{
			std::pair<std::string, ObjectPos> anchorPointEntry{};

			bool firstItem = false;
			for (auto begin = (*offsetPairing)->GetMaps()->begin(); begin != (*offsetPairing)->GetMaps()->end(); ++begin)
			{
				for (auto item : list)
				{
					if (begin->first == item.first)
					{
						firstItem = true;
						anchorPointEntry = (*begin);
						goto forexit;
					}
				}

			}
		forexit:
			if (!firstItem) {
				anchorPointEntry = (*(*offsetPairing)->GetMaps()->begin());
			}
			ObjectPos anchorPoint{};
			for (auto find : list)
			{
				if (find.first == anchorPointEntry.first)
				{
					anchorPoint = find.second;
				}
			}
			ObjectPos vectorPoint = anchorPointEntry.second;
			ObjectPos fromAnchorPoint{ anchorPoint.x - vectorPoint.x, anchorPoint.y - vectorPoint.y };

			for (auto childAnchorPoint = (*offsetPairing)->GetMaps()->begin(); childAnchorPoint != (*offsetPairing)->GetMaps()->end(); ++childAnchorPoint)
			{
				if (childAnchorPoint->first != anchorPointEntry.first)
				{
					bool check = false;
					for (auto item : list) 
					{
						if (item.first == childAnchorPoint->first) {
							check = true;
						}
					}
					if (!check) 
					{
						list.push_back({ childAnchorPoint->first,
							{ fromAnchorPoint.x + childAnchorPoint->second.x,
							  fromAnchorPoint.y + childAnchorPoint->second.y } });
					}
				}
			}
			offsetPairing = offsets.erase(offsetPairing);
		}

		auto neckOffsetBody = bodyFrame->GetMaps()->find("neck")->second;
		auto navelOffsetBody = bodyFrame->GetMaps()->find("navel")->second;
		std::list<std::pair<SkinParts*, ObjectPos>> positionedFramesList;
		for (auto positionedFrame : partsFrames)
		{
			auto fromAnchorPoint = neckOffsetBody;
			if (positionedFrame->GetMaps()->size() > 0)
			{
				auto anchorPointEntry = positionedFrame->GetMaps()->begin();
				if (anchorPointEntry == positionedFrame->GetMaps()->end())
				{
					break;
				}
				ObjectPos anchorPoint{};
				bool check = false;
				for (auto item : list) {
					if (anchorPointEntry->first == item.first)
					{
						anchorPoint = item.second;
					}
				}

				auto vectorFromPoint = anchorPointEntry->second;
				fromAnchorPoint = { anchorPoint.x - vectorFromPoint.x, anchorPoint.y - vectorFromPoint.y };
			}
			std::unordered_map<std::string, ObjectPos> maplist;
			for (auto begin = positionedFrame->GetMaps()->begin();  begin != positionedFrame->GetMaps()->end(); ++begin)
			{
				for (auto item : list) {

					if (begin->first != item.first)
					{
						maplist.insert((*begin));
					}
				}
			}
			ObjectPos vectorFromPoint{};
			if (!maplist.empty())
			{
				vectorFromPoint = maplist.begin()->second;
			}
			

			auto partOrigin = positionedFrame->GetOrigin();
			auto tempPos = ObjectPos{ fromAnchorPoint.x - partOrigin.x, fromAnchorPoint.y - partOrigin.y };
			positionedFramesList.push_back({ positionedFrame, tempPos });
		}
		auto minXPair = std::min_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.x < rhs.second.x;
			});
		auto minX = minXPair->second.x;

		auto maxXPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.x + lhs.first->GetBitmap()->GetWidth() < rhs.second.x + rhs.first->GetBitmap()->GetWidth();
			});
		auto maxX = maxXPair->second.x + maxXPair->first->GetBitmap()->GetWidth();

		auto minYPair = std::min_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.y < rhs.second.y;
			});
		auto minY = minYPair->second.y;

		auto maxYPair = std::max_element(positionedFramesList.begin(),
			positionedFramesList.end(),
			[](const std::pair<SkinParts*, ObjectPos>& lhs, const std::pair<SkinParts*, ObjectPos>& rhs) {
				return lhs.second.y + lhs.first->GetBitmap()->GetHeight()  < rhs.second.y + rhs.first->GetBitmap()->GetHeight();
			});

		auto maxY = maxYPair->second.y + maxYPair->first->GetBitmap()->GetHeight();


		if (bodyFrame->GetMaps()->find("neck") != bodyFrame->GetMaps()->end())
		{
			int plus = (_is_prone ? 25 : 0);
			HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
			HBRUSH brushPrev = (HBRUSH)SelectObject(_memDC, brush);
			Rectangle(_memDC, -30, -30, _info.cx + 120, _info.cy + 120);
			SelectObject(_memDC, brushPrev);
			DeleteObject(brush);
			DeleteObject(brushPrev);

			ObjectPos centerPos{ (maxX - minX) / 2, (maxY - minY) / 2 };

			ObjectPos destination{ (maxX - minX) + (2 * 2), (maxY - minY) + (2 * 2) };
			//int oldCx = _info.cx - static_cast<int>(destination.x);
			int oldCy = _info.cy - static_cast<int>(destination.y);
			//if (oldCx != 0) {
			//	//_info.x += oldCx >> 1;
			//}
			if (oldCy != 0) {
				_info.y += oldCy >> 1;
			}
			//_info.cx = static_cast<int>(destination.x);
			_info.cy = static_cast<int>(destination.y);
			UpdateRectGameObject();

			std::pair<SkinParts*, ObjectPos> lastBody;
			for (auto body : positionedFramesList)
			{
				if ((!strcmp(body.first->GetName().c_str(), "body")) || !strcmp(body.first->GetName().c_str(), "backBody") &&
					body.first->GetMaps()->find("neck") != body.first->GetMaps()->end() &&
					body.first->GetMaps()->find("navel") != body.first->GetMaps()->end()) {
					lastBody = body;
				}
			}
			//ObjectPos bodyShouldBe{ 36, 55 };
			//float tempX1 = lastBody.second.x - bodyShouldBe.x;
			//float tempY1 = lastBody.second.y - bodyShouldBe.y;
			//float tempX2 = minX;
			//float tempY2 = minY;



			//ObjectPos cropOrigin{ tempX1 - tempX2 , tempY1 - tempY2 };
			//RECT cropArea{ static_cast<int>(std::fmax(cropOrigin.x, 0)),
			//	 static_cast<int>(std::fmax(cropOrigin.y, 0)), 96, 96 };
			//ObjectPos cropOffsetFromOrigin{ cropArea.left - cropOrigin.x, cropArea.top - cropOrigin.y };

			for (auto draw : positionedFramesList)
			{
				draw.first->GetBitmap()->RenderBitmapImage(_memDC,
					static_cast<int>(std::floor(draw.second.x - minX)),
					static_cast<int>(std::floor(draw.second.y - minY)),
					static_cast<int>(draw.first->GetBitmap()->GetWidth()),
					static_cast<int>(draw.first->GetBitmap()->GetHeight()));
			}//var cropArea = new Rectangle((int)Math.Max(cropOrigin.X, 0), (int)Math.Max(cropOrigin.Y, 0), 96, 96);
			//ObjectPos Temp{ positionedFramesList}
			// Point.Subtract(Point.Subtract(body.Item2, bodyShouldBe), new Size((int)minX, (int)minY))
			if (GetFacingDirection())
			{
				int x = static_cast<int>(destination.x);
				int y = static_cast<int>(destination.y);
				StretchBlt(_memDC, 0, 0, x, y, _memDC, x - 1, 0, -x, y, SRCCOPY);
			}
			/*else
			{
				int x = static_cast<int>(destination.x);
				int y = static_cast<int>(destination.y);
				StretchBlt(hdc, 0, 0, x, y, _memDC, 0, 0, x, y, SRCCOPY);
			}*/

			/*Rectangle(hdc,
				static_cast<int>(std::floor(_rect.left + ScrollManager::GetScrollX())),
				static_cast<int>(std::floor(_rect.top + ScrollManager::GetScrollY())),
				static_cast<int>(std::floor(_rect.right + ScrollManager::GetScrollX())),
				static_cast<int>(std::floor(_rect.bottom + ScrollManager::GetScrollY())));*/
			
			UpdateRectGameObject();

			auto footOriginX = lastBody.first->GetOrigin().x + (lastBody.second.x - minX);
			auto footOriginY = lastBody.first->GetOrigin().y + (lastBody.second.y - minY);
			float reduceX= _info.x - (_rect.right - footOriginX);
			float reduceY= _info.y - (_rect.bottom - footOriginY);
			if (GetFacingDirection())
			{
				auto reversOriginX = (std::abs(lastBody.first->GetOrigin().x - destination.x)) + ((lastBody.second.x - minX) * -1);
				float reduceX = _info.x - (_rect.right - reversOriginX);
				float reduceY = _info.y - (_rect.bottom - footOriginY);
				GdiTransparentBlt(hdc,
					static_cast<int>(_rect.left - reduceX + ScrollManager::GetScrollX()),
					static_cast<int>(_info.y - reduceY + ScrollManager::GetScrollY()),
					static_cast<int>(destination.x),
					static_cast<int>(destination.y),
					_memDC,
					0,
					0,
					static_cast<int>(destination.x),
					static_cast<int>(destination.y),
					RGB(255, 0, 255));
				/*Rectangle(hdc,
					static_cast<int>(_rect.left + ScrollManager::GetScrollX() + reduceX),
					static_cast<int>(_rect.top + ScrollManager::GetScrollY() + (발오리진y)),
					static_cast<int>(_rect.left + ScrollManager::GetScrollX() + reduceX ) + 10,
					static_cast<int>(_rect.top + ScrollManager::GetScrollY() + (발오리진y)) + 10);*/
			}
			else
			{
				GdiTransparentBlt(hdc,
					static_cast<int>(_rect.left - reduceX + ScrollManager::GetScrollX()) ,
					static_cast<int>(_info.y - reduceY + ScrollManager::GetScrollY()),
					static_cast<int>(destination.x),
					static_cast<int>(destination.y),
					_memDC,
					0,
					0,
					static_cast<int>(destination.x),
					static_cast<int>(destination.y),
					RGB(255, 0, 255));
				/*Rectangle(hdc,
					static_cast<int>(_rect.left + ScrollManager::GetScrollX() + (발오리진x)),
					static_cast<int>(_rect.top + ScrollManager::GetScrollY() + (발오리진y)),
					static_cast<int>(_rect.left + ScrollManager::GetScrollX() + (발오리진x)) + 10,
					static_cast<int>(_rect.top + ScrollManager::GetScrollY() + (발오리진y)) + 10);*/
			}
			std::wstring str;
			//str.append(L"X: ").append(std::to_wstring(_info.x)).append(L"  Y : ").append(std::to_wstring(_info.y));
			///TextOut(hdc, static_cast<int>(_rect.left - reduceX + ScrollManager::GetScrollX()),
			//	static_cast<int>(_info.y - reduceY + ScrollManager::GetScrollY()), str.c_str(), static_cast<int>(str.size()));
		}
	}
}

void Player::IsJumping()
{
	if(_is_rope) 
	{
		float outY = 0;
		_is_first_foothold = true;
		_is_jump = false;
		bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &_next_foothold);
		return;
	}
	if (GetTickCount64() < _knockback_tick + _knockback_time)
	{ 
		return;
	}
	float outY = 0;
	bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &_now_foothold);

	const int count = 17;
	const float speed = 6.f;
	if (_is_jump)
	{
		++_jump_count;
		if (_jump_count < count)
		{
			_info.y -= speed;
			//ScrollManager::GainScrollY(speed, true);
		}
		else
		{
			_is_jump = false;
			_is_fly = true;
			_jump_count = 0;
		}
	}
	else if (!_is_rope && isFoothold)
	{
		if (_info.y <= outY)
		{
			if (_info.y - outY <= speed && _info.y - outY >= -speed)
			{
				_info.y = outY;
				_is_fly = false;
				if (_is_attacking)
				{
					if (strcmp(GetFrameState(), "swingO2")) //공격모션이아니라면 공격품
					{
						_is_attacking = false;
					}
				}
				//ScrollManager::SetScrollY(-_info.y + (768 / 2));
			}
			else
			{
				if (!_is_attacking)
				{
					this->ChangeFrameState("jump");
				}
				_is_fly = true;
				_info.y += speed;
				//ScrollManager::GainScrollY(-speed);
			}
		}
		else
		{
			_is_fly = false;
			_info.y = outY;
			//ScrollManager::SetScrollY(-_info.y + (768 / 2));
		}
	}
	//float fY = 0.f;
	//bool bIsColl = MapManager::GetInstance()->FootholdCollision(this, &fY);

	//if (_isJump)
	//{
	//	_accel += 0.20f;
	//	_info.y -= _jumpPower * _accel - _gravity * _accel * _accel * 0.5f;
	//	int i = 0;
	//	if (bIsColl && _info.y >= fY)
	//	{
	//		_isJump = false;
	//		_accel = 0.f;
	//		_info.y = fY;
	//	}
	//}
	//else if (bIsColl)
	//	_info.y = fY;
}

void Player::TryMeleeAttack()
{
	if (GetFacingDirection())
	{
		_melee_attack_hitbox = {
			static_cast<int>(_info.x + -5),
			static_cast<int>(_info.y + -50),
			static_cast<int>(_info.x + 73),
			static_cast<int>(_info.y + 17) };
	}
	else
	{
		_melee_attack_hitbox = {
			static_cast<int>(_info.x + -73),
			static_cast<int>(_info.y + -50),
			static_cast<int>(_info.x +  5),
			static_cast<int>(_info.y +  17) };
	}
	_is_attacking = true;
}

bool Player::IsAlertStateTick()
{
	auto tick = GetTickCount64();
	if (tick < _alert_tick + 5000) 
	{
		return true;
	}
	return false;
}

void Player::UpdateAlertTick()
{
	_alert_tick = GetTickCount64();
}

void Player::AttackMonster(Monster* monster)
{
	//TODO: 데미지연산은 여기서..처리하도록하자
	int32_t damage = rand();
	monster->GainHp(-damage);
	if (!monster->IsAlive())
	{
		GetPlayerInfo()->exp += monster->GetExp();
	}
	_damage_handler->InsertAttackDamageEffect(monster, damage, 1000);
	monster->ChangeState(Monster::MonsterState::kHit);
}

bool Player::IsInvincibility()
{
	if (GetTickCount64() > _take_damage_tick + 3000)
	{
		return false;
	}
	return true;
}

void Player::SetInvincibility()
{
	_take_damage_tick = GetTickCount64();
	_alert_tick = GetTickCount64();
}

void Player::ResetEqpStat()
{
	_player_info.eqp_str = 0;
	_player_info.eqp_dex = 0;
	_player_info.eqp_int_ = 0;
	_player_info.eqp_luk = 0;
	_player_info.eqp_max_hp = 0;
	_player_info.eqp_max_mp = 0;
	_player_info.eqp_pad = 0;
	_player_info.eqp_mad = 0;
	_player_info.eqp_pdd = 0;
	_player_info.eqp_mdd = 0;
	_player_info.min_power = 0;
	_player_info.max_power = 0;
}

void Player::RecalcEqpStat()
{
	ResetEqpStat();
	for (auto& item_pair : _equipment->GetEquipItems())
	{
		auto item = item_pair.second;
		if (item->GetItemInfo().GetIncStr() > 0)
		{
			this->GainEqpStr(item->GetItemInfo().GetIncStr());
		}
		if (item->GetItemInfo().GetIncDex() > 0)
		{
			this->GainEqpDex(item->GetItemInfo().GetIncDex());
		}
		if (item->GetItemInfo().GetIncInt() > 0)
		{
			this->GainEqpInt(item->GetItemInfo().GetIncInt());
		}
		if (item->GetItemInfo().GetIncLuk() > 0)
		{
			this->GainEqpLuk(item->GetItemInfo().GetIncLuk());
		}
		if (item->GetItemInfo().GetIncPad() > 0)
		{
			this->GainEqpPad(item->GetItemInfo().GetIncPad());
		}
		if (item->GetItemInfo().GetIncMad() > 0)
		{
			this->GainEqpMad(item->GetItemInfo().GetIncMad());
		}
		if (item->GetItemInfo().GetIncPdd() > 0)
		{
			this->GainEqpPdd(item->GetItemInfo().GetIncPdd());
		}
		if (item->GetItemInfo().GetIncMdd() > 0)
		{
			this->GainEqpMdd(item->GetItemInfo().GetIncMdd());
		}
		if (item->GetItemInfo().GetIncMhp() > 0)
		{
			this->GainEqpMaxHp(item->GetItemInfo().GetIncMhp());
		}
		if (item->GetItemInfo().GetIncMmp() > 0)
		{
			this->GainEqpMaxMp(item->GetItemInfo().GetIncMmp());
		}
	}

	uint32_t min_power = 0;
	min_power += 3 * this->GetTotalStr();
	min_power += 5 * this->GetEqpPad();
	this->GainMinPower(min_power);
	uint32_t max_power = 0;
	max_power += 6 * this->GetTotalStr();
	max_power += 10 * this->GetEqpPad();
	this->GainMaxPower(max_power);
}

void Player::TakeDamage()
{
	auto mosnters = MapManager::GetInstance()->MonsterCollision(_rect, 1);

	if (!mosnters.empty() && GetTickCount64() > _take_damage_tick + 3000)
	{
		this->GainHp(-1);
		_take_damage_tick = GetTickCount64();
		_alert_tick = GetTickCount64();
		_damage_handler->InsertTakeDamageEffect(this, 1234, 1000);
		if ((*mosnters.begin())->GetInfo().x > this->GetInfo().x)
		{
			SettingPushKnockBack(false);
		}
		else
		{
			SettingPushKnockBack(true);
		}
		
	}

}

void Player::SettingPushKnockBack(bool fancing)
{
	_knockback_facing_direction = fancing;
	_knockback_tick = GetTickCount64();
}

Inventory* Player::GetInventory(::ObjectType::InventoryTabState type)
{
	return _inventory[type];
}

EqpInventory* Player::GetEqpInventory()
{
	return _eqp_inventory;
}

Equipment* Player::GetEquipment()
{
	return _equipment;
}

void Player::ApplySkill()
{
	const auto key_manager = KeyManager::GetInstance();
	const auto quick_slot = UiManager::GetInstance()->GetQuickSlot();
	QuickSlot::KeyBoard key_board = QuickSlot::KeyBoard::kEnd;
	if (key_manager->KeyPressing(KEY_SHIFT))
	{
		key_board = QuickSlot::KeyBoard::kShift;
	}
	else if (key_manager->KeyPressing(KEY_INSERT))
	{
		key_board = QuickSlot::KeyBoard::kInsert;
	}
	else if (key_manager->KeyPressing(KEY_HOME))
	{
		key_board = QuickSlot::KeyBoard::kHome;
	}
	else if (key_manager->KeyPressing(KEY_PAGEUP))
	{
		key_board = QuickSlot::KeyBoard::kPageUp;
	}
	else if (key_manager->KeyPressing(KEY_CTRL))
	{
		key_board = QuickSlot::KeyBoard::kControl;
	}
	else if (key_manager->KeyPressing(KEY_DELETE))
	{
		key_board = QuickSlot::KeyBoard::kDelete;
	}
	else if (key_manager->KeyPressing(KEY_END))
	{
		key_board = QuickSlot::KeyBoard::kEndKey;
	}
	else if (key_manager->KeyPressing(KEY_PAGEDOWN))
	{
		key_board = QuickSlot::KeyBoard::kPageDown;
	}

	if (key_board != QuickSlot::KeyBoard::kEnd)
	{
		const auto attack_skill = quick_slot->GetSkill(key_board);
		if (attack_skill != nullptr)
		{
			auto skill_info = attack_skill->GetSkillInfo()[attack_skill->GetSkillInfo().size() - 1];

			RECT rect{
				static_cast<long>(_info.x) - std::abs(skill_info->GetRect().left),
				static_cast<long>(_info.y) - std::abs(skill_info->GetRect().top),
				static_cast<long>(_info.x) + std::abs(skill_info->GetRect().right) ,
				static_cast<long>(_info.y) + std::abs(skill_info->GetRect().bottom) };

			if (skill_info->GetRect().left == 0 && skill_info->GetRect().right == 0 && skill_info->GetRect().top == 0 && skill_info->GetRect().bottom == 0)
			{
				if (GetFacingDirection())
				{
					rect = {
						static_cast<long>(_info.x),
						static_cast<long>(_info.y) - 65,
						static_cast<long>(_info.x) + 200,
						static_cast<long>(_info.y) + 65 };
				}
				else
				{
					rect = {
						static_cast<long>(_info.x) + -200 ,
						static_cast<long>(_info.y) + -65,
						static_cast<long>(_info.x),
						static_cast<long>(_info.y) + 65 };
				}
			}

			const auto mosnters = MapManager::GetInstance()->MonsterCollision(rect, skill_info->GetMobCount());

			if (_attack_skill == nullptr)
			{
				_attack_skill = new MagicAttack(attack_skill, this, mosnters);
				_attack_skill->ReadyMagicAttack();
			}
			else
			{
				_attack_skill->ResetSkill(attack_skill, this, mosnters);
			}
			this->ChangeFrameState("swingO2");
			UpdateAlertTick();
			_is_attacking = true;
			for (auto& monster : mosnters)
			{
				std::list<uint32_t> damages;
				for (int i = 0; i < skill_info->GetAttackCount(); ++i)
				{
					damages.emplace_back(rand());
				}
				_damage_handler->InsertAttackDamageEffect(monster, damages, 1000);
				monster->ChangeState(Monster::MonsterState::kHit);
				monster->SetPlayer(this);
			}
			return;
		}
		const auto use_item = quick_slot->GetItem(key_board);
		if (use_item != nullptr && GetTickCount64() > _use_item_tick + 500)
		{
			if (use_item->GetQuantity() <= 0)
			{
				return;
			}
			for (auto& spec : use_item->GetSpec())
			{
				if (!strcmp(spec.first.c_str(), "hp"))
				{
					this->GainHp(spec.second);
				}
				else if (!strcmp(spec.first.c_str(), "mp"))
				{
					this->GainMp(spec.second);
				}
				else if (!strcmp(spec.first.c_str(), "hpR"))
				{
					this->GainHp(this->GetMaxHp() * spec.second / 100);
				}
				else if (!strcmp(spec.first.c_str(), "mpR"))
				{
					this->GainMp(this->GetMaxMp() * spec.second / 100);
				}
			}
			use_item->GainQuantity(-1);
			_use_item_tick = GetTickCount64();
		}
	}
}

void Player::RenderGameObject(HDC hdc)
{
	UpdateRectGameObject();
	RenderCharacter(hdc);
	_damage_handler->ShowDamages(hdc);
	if (_attack_skill != nullptr)
	{
		_attack_skill->SkillRender(hdc);
	}
}

void Player::LateUpdateGameObject()
{
	Player::IsJumping();

	uint64_t tick = GetTickCount64();
	if (_frame_this != nullptr)
	{
		if (!_is_rope && tick > _frame_tick + _frame_this->GetDelay())
		{

			if (!strcmp(GetFrameState(), "stand1") || !strcmp(GetFrameState(), "swingO2") || !strcmp(GetFrameState(), "alert"))
			{
				if (_frame_revers)
				{
					if (--_frame_nummber == 0)
					{
						_frame_revers = false;
					}
				}
				else
				{
					if (++_frame_nummber >= _frame_this->GetPartner()->GetSkinFrames()->size())
					{
						_frame_revers = true;
					}
				}
				if (_is_attacking)
				{
					if (_frame_nummber >= _frame_this->GetPartner()->GetSkinFrames()->size())
					{
						this->ChangeFrameState("alert");
						auto mosnters = MapManager::GetInstance()->MonsterCollision(_melee_attack_hitbox, 1);
						for (auto monster : mosnters)
						{
							AttackMonster(monster);
							//std::cout << "몬스터 데미지 피격!" << monster->GetHp() << std::endl;
						}
						_is_attacking = false;
					}
				}
			}
			else
			{
				++_frame_nummber;
			}
			_frame_tick = tick;
		}
	}
}

void Player::ScrollMove()
{
	const int range = 50;

	const int divideCX = WindowCX >> 1;
	const int divideCY = WindowCY >> 1;


	const float scrollX = ScrollManager::GetScrollX();
	const float scrollY = ScrollManager::GetScrollY();
	const float  sumX = _info.x + scrollX;
	const float  sumY = _info.y + scrollY;

	if (divideCX < sumX + range)
	{
		ScrollManager::GainScrollX(-GetSpeed());
	}
	if (divideCX > sumX - range)
	{
		ScrollManager::GainScrollX(GetSpeed());
	}

	if (divideCY < sumY + range)
	{
		ScrollManager::GainScrollY(-GetSpeed());
	}
	if (divideCY > sumY - range)
	{
		ScrollManager::GainScrollY(GetSpeed());
	}

}

void Player::SetFrameThis(SkinFrame* frame)
{
	_frame_this = frame;
}

SkinFrame* Player::GetFrameThis()
{
	return _frame_this;
}

std::vector<SkinInfo*> Player::FindSkinFrame() const
{

	return std::vector<SkinInfo*>();
}

void Player::InsertFrame(SkinInfo* item)
{
}

void Player::ChangeFrameState(std::string frame)
{
	if (strcmp(GetFrameState(), frame.c_str()))
	{
		LoadCharacterFrame(frame);
		_frame_state = frame;
		_frame_nummber = 0;
		_frame_revers = false;
	}
}

const char* Player::GetFrameState() const
{
	//char findStr[30];
	//snprintf(findStr, 30, "%s/%d", _frameState.c_str(), _frameNummber);

	//return findStr;
	return _frame_state.c_str();
}

uint16_t Player::GetFrameNummber(uint16_t remain)
{
	return _frame_nummber % remain;
}

void Player::SetFacingDirection(uint8_t direction)
{
	_facing_direction = direction;
}

uint8_t Player::GetFacingDirection() const
{
	return _facing_direction;
}

FootHold* Player::GetNowFootHold() const
{
	return _now_foothold;
}

void Player::SetNowFootHold(FootHold* hold)
{
	_now_foothold = hold;
}

ObjectInfo* Player::GetPlayerInfo()
{
	return &_player_info;
}

int16_t Player::GetHp() const
{
	return _player_info.hp;
}

int16_t Player::GetMaxHp() const
{
	return _player_info.max_hp;
}

int16_t Player::GetEqpMaxHp() const
{
	return _player_info.eqp_max_hp;
}

int16_t Player::GetTotalMaxHp() const
{
	return _player_info.max_hp + _player_info.eqp_max_hp;
}

int16_t Player::GetMp() const
{
	return _player_info.mp;
}

int16_t Player::GetMaxMp() const
{
	return _player_info.max_mp;
}

int16_t Player::GetEqpMaxMp() const
{
	return _player_info.eqp_max_mp;
}

int16_t Player::GetTotalMaxMp() const
{
	return _player_info.max_mp + _player_info.eqp_max_mp;
}

int16_t Player::GetAp() const
{
	return _player_info.ap;
}

int16_t Player::GetSp() const
{
	return _player_info.sp;
}

int16_t Player::GetJob() const
{
	return _player_info.job;
}

int16_t Player::GetLevel() const
{
	return _player_info.level;
}

int32_t Player::GetExp() const
{
	return _player_info.exp;
}

int16_t Player::GetStr() const
{
	return _player_info.str;
}

int16_t Player::GetDex() const
{
	return _player_info.dex;
}

int16_t Player::GetInt() const
{
	return _player_info.int_;
}

int16_t Player::GetLuk() const
{
	return _player_info.luk;
}

int16_t Player::GetEqpStr() const
{
	return _player_info.eqp_str;
}

int16_t Player::GetEqpDex() const
{
	return _player_info.eqp_dex;
}

int16_t Player::GetEqpInt() const
{
	return _player_info.eqp_int_;
}

int16_t Player::GetEqpLuk() const
{
	return _player_info.eqp_luk;
}

int16_t Player::GetTotalStr() const
{
	return _player_info.str + _player_info.eqp_str;
}

int16_t Player::GetTotalDex() const
{
	return _player_info.dex + _player_info.eqp_dex;
}

int16_t Player::GetTotalInt() const
{
	return _player_info.int_ + _player_info.eqp_int_;
}

int16_t Player::GetTotalMad() const
{
	return _player_info.int_ + _player_info.eqp_int_ + _player_info.eqp_mad;
}

int16_t Player::GetTotalLuk() const
{
	return _player_info.luk + _player_info.eqp_luk;
}

int16_t Player::GetEqpPad() const
{
	return _player_info.eqp_pad;
}

int16_t Player::GetEqpMad() const
{
	return _player_info.eqp_mad;
}

int16_t Player::GetEqpPdd() const
{
	return _player_info.eqp_pdd;
}

int16_t Player::GetEqpMdd() const
{
	return _player_info.eqp_mdd;
}

uint32_t Player::GetMinPower() const
{
	return _player_info.min_power;
}

uint32_t Player::GetMaxPower() const
{
	return _player_info.max_power;
}

int32_t Player::GetMeso() const
{
	return _player_info.meso;
}


std::string Player::GetName() const
{
	return _player_info.name;
}

std::shared_ptr<QuestInfo> Player::FindClearQuest(int32_t npc_id)
{
	auto data = _clear_quest_list.find(npc_id);
	if (data != _clear_quest_list.end())
	{
		return data->second;
	}
	return nullptr;
}

void Player::InsertClearQuest(std::shared_ptr<QuestInfo> info)
{
	_clear_quest_list.insert(std::make_pair(info->GetNpcId(), info));
}

void Player::RemoveClearQuest(std::shared_ptr<QuestInfo> info)
{
	_clear_quest_list.erase(info->GetNpcId());
}

void Player::RemoveIngQuest(std::shared_ptr<QuestInfo> info)
{
	_ing_quest_list.erase(info->GetNpcId());
}

std::shared_ptr<QuestInfo> Player::FindIngQuest(int32_t npc_id)
{
	auto data = _ing_quest_list.find(npc_id);
	if (data != _ing_quest_list.end())
	{
		return data->second;
	}
	return nullptr;
}

void Player::InsertIngQuest(std::shared_ptr<QuestInfo> info)
{
	_ing_quest_list.insert(std::make_pair(info->GetNpcId(), info));
}

void Player::SetJob(const int16_t value)
{
	_player_info.job = value;
}

void Player::GainLevel(int16_t value)
{
	_player_info.level += value;
}

void Player::GainExp(int16_t value)
{
	_player_info.exp += value;
}

void Player::GainEqpStr(int16_t value)
{
	_player_info.eqp_str += value;
}

void Player::GainEqpDex(int16_t value)
{
	_player_info.eqp_dex += value;
}

void Player::GainEqpInt(int16_t value)
{
	_player_info.eqp_int_ += value;
}

void Player::GainEqpLuk(int16_t value)
{
	_player_info.eqp_luk += value;
}

void Player::GainEqpPad(int16_t value)
{
	_player_info.eqp_pad += value;
}

void Player::GainEqpMad(int16_t value)
{
	_player_info.eqp_mad += value;
}

void Player::GainEqpPdd(int16_t value)
{
	_player_info.eqp_pdd += value;
}

void Player::GainEqpMdd(int16_t value)
{
	_player_info.eqp_mdd += value;
}

void Player::GainMinPower(uint32_t value)
{
	_player_info.min_power += value;
}

void Player::GainMaxPower(uint32_t value)
{
	_player_info.max_power += value;
}

void Player::GainMeso(uint32_t value)
{
	_player_info.meso += value;
}

DamageHandler* Player::GetDamageHandler() const
{
	return _damage_handler;
}

void Player::GainHp(const int16_t value)
{
	_player_info.hp += value;
	if (_player_info.hp  >= _player_info.max_hp)
	{
		_player_info.hp = _player_info.max_hp;
	}
}

void Player::GainEqpMaxHp(int16_t value)
{
	_player_info.eqp_max_hp += value;
}

void Player::GainMaxHp(const int16_t value)
{
	_player_info.max_hp += value;
}

void Player::GainMp(const int16_t value)
{
	_player_info.mp += value;
	if (_player_info.mp >= _player_info.max_mp)
	{
		_player_info.mp = _player_info.max_mp;
	}
}

void Player::GainEqpMaxMp(int16_t value)
{
	_player_info.eqp_max_mp += value;
}

void Player::GainMaxMp(const int16_t value)
{
	_player_info.max_mp += value;
}

void Player::SetHp(const int16_t value)
{
	_player_info.hp = value;
}

void Player::SetMaxHp(const int16_t value)
{
	_player_info.max_hp = value;
}

void Player::SetMp(const int16_t value)
{
	_player_info.mp = value;
}

void Player::SetMaxMp(const int16_t value)
{
	_player_info.max_mp = value;
}

void Player::GainAp(const int16_t value)
{
	_player_info.ap += value;
}

void Player::GainSp(const int16_t value)
{
	_player_info.sp += value;
}

void Player::GainStr(int16_t value)
{
	_player_info.str += value;
}

void Player::GainDex(int16_t value)
{
	_player_info.dex += value;
}

void Player::GainInt(int16_t value)
{
	_player_info.int_ += value;
}

void Player::GainLuk(int16_t value)
{
	_player_info.luk += value;
}
