#include "../../../pch.h"
#include "monster.h"

#include "../foot_hold.h"
#include "../Player/player.h"
#include "AiMovement/ai_movement.h"
#include "MonsterUtility/monster_frame_manager.h"
#include "../../../Managers/MonsterMnager/monster_manager.h"
#include "../../../Managers/MonsterMnager/monster_movement.h"
#include "../../../Managers/MonsterMnager/monster_parts.h"
#include "../../../Managers/ScrollManager/scroll_manager.h"
#include "../../../Managers/MapManager/map_manager.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../Player/Damage/damage_handler.h"
#include "AttackInfo/attack_info.h"
#include "../../../Managers/DropDataManager/drop_data_manager.h"

Monster::Monster() :
	GameObject(0),
	_monster_info({}),
	_now_foothold(nullptr),
	_monster_state(MonsterState::kStand),
	_facing_direction(false),
	_is_alive(true),
	_die_wait_tick(0),
	_alpha_value(255),
	_alpha_tick(0),
	_is_drop_item(false),
	_base_state_frame(nullptr),
	_this_foothold(nullptr),
	_ai_movement(nullptr),
	_target(nullptr),
	_memDC2(nullptr),
	_bitmap2(nullptr),
	_old_bitmap2(nullptr),
	_is_attack(false)


{
	InsertAttackDelay("attack1", GetTickCount64());
	InsertAttackDelay("attack2", GetTickCount64() + 10000);
	InsertAttackDelay("attack3", GetTickCount64() + 20000);
}

Monster::~Monster()
{
	SelectObject(_memDC, _old_bitmap);
	DeleteObject(_bitmap);
	DeleteDC(_memDC);
	SelectObject(_memDC2, _old_bitmap2);
	DeleteObject(_bitmap2);
	DeleteDC(_memDC2);
	delete _ai_movement;
	delete _base_state_frame;
}

MonsterInfo Monster::GetMonsterInfo()
{
	return _monster_info;
}

void Monster::SetMonsterCode(std::string code)
{
	_monster_code = code;
}

std::string Monster::GetMonsteCode() const
{
	return _monster_code;
}

void Monster::SetMonsterId(int32_t code)
{
	_monster_id = code;
}

int32_t Monster::GetMonsterId() const
{
	return _monster_id;
}

void Monster::SetLevel(uint8_t level)
{
	_monster_info.level = level;
}

uint8_t Monster::GetLevel() const
{
	return _monster_info.level;
}

void Monster::SetSpeed(float speed)
{
	_monster_info.speed = speed;
}

float Monster::GetSpeed() const
{
	return _monster_info.speed;
}

void Monster::SetUndead(uint8_t undead)
{
	_monster_info.undead = undead;
}

uint8_t Monster::GetUndead() const
{
	return _monster_info.undead;
}

void Monster::SetBodyAttack(uint8_t body)
{
	_monster_info.body_attack = body;
}

uint8_t Monster::GetBodyAttack() const
{
	return _monster_info.body_attack;
}

void Monster::SetBoss(uint8_t boss)
{
	_monster_info.boss = boss;
}

bool Monster::IsBoss() const
{
	return _monster_info.boss;
}

void Monster::SetPad(uint16_t pad)
{
	_monster_info.pad = pad;
}

uint16_t Monster::GetPad() const
{
	return _monster_info.pad;
}

void Monster::SetMad(uint16_t mad)
{
	_monster_info.mad = mad;
}

uint16_t Monster::GetMad() const
{
	return _monster_info.mad;
}

void Monster::SetPdd(uint16_t pdd)
{
	_monster_info.pdd = pdd;
}

uint16_t Monster::GetPdd() const
{
	return _monster_info.pdd;
}

void Monster::SetMdd(uint16_t mdd)
{
	_monster_info.mdd = mdd;
}

uint16_t Monster::GetMdd() const
{
	return _monster_info.mdd;
}

void Monster::SetExp(uint32_t exp)
{
	_monster_info.exp = exp;
}

uint32_t Monster::GetExp() const
{
	return _monster_info.exp;
}

void Monster::SetHp(uint32_t hp)
{
	_monster_info.hp = hp;
	if (0 > _monster_info.hp)
	{
		_is_alive = false;
	}
}

uint32_t Monster::GetHp() const
{
	return _monster_info.hp;
}

void Monster::GainHp(const int32_t hp)
{
	_monster_info.hp += hp;
	if (0 > _monster_info.hp)
	{
		_is_alive = false;
	}
}

void Monster::SetMaxHp(uint32_t maxhp)
{
	_monster_info.max_hp = maxhp;
}

uint32_t Monster::GetMaxHp() const
{
	return  _monster_info.max_hp;
}

void Monster::SetMp(uint32_t mp)
{
	_monster_info.mp = mp;
}

uint32_t Monster::GetMp() const
{
	return _monster_info.mp;
}

void Monster::GainMp(const int32_t mp)
{
	_monster_info.mp += mp;
}

void Monster::SetMaxMp(const uint32_t maxmp)
{
	_monster_info.max_mp = maxmp;
}

uint32_t Monster::GetMaxMp() const
{
	return _monster_info.max_mp;
}

void Monster::SetPlayer(GameObject* player)
{
	_target = player;
}

GameObject* Monster::GetPlayer() const
{
	return _target;
}

void Monster::InsertAttackInfo(const std::string& key, AttackInfo* attack_info)
{
	_attack_info.insert(std::make_pair(key, attack_info));
}

AttackInfo* Monster::FindAttackInfo(const std::string key) const
{
	auto data = _attack_info.find(key);

	if (data != _attack_info.end())
	{
		return data->second;
	}
	return nullptr;
}

std::map<std::string, AttackInfo*>& Monster::GetAttackInfo()
{
	return _attack_info;
}

void Monster::SetMovement(const std::shared_ptr<MonsterMovement> movement)
{
	_movement = movement;
	_this_frame = _movement->FindMovement("stand");
}

std::shared_ptr<MonsterMovement> Monster::GetMovement() const
{
	return _movement;
}

void Monster::SetName(const std::string name)
{
	_monster_info.name = name;
}

std::string Monster::GetName() const
{
	return _monster_info.name;
}

void Monster::InsertAttackDelay(std::string key, uint64_t tick)
{
	auto data = _attack_delay.find(key);
	if (data != _attack_delay.end())
	{
		data->second = tick;
		return;
	}
	_attack_delay.insert(std::make_pair(key, tick));
}

uint64_t Monster::FindAttackDelay(std::string key)
{
	auto data = _attack_delay.find(key);
	if (data != _attack_delay.end())
	{
		return data->second;
	}
	return 0;
}

size_t Monster::GetAttackDelaySize() const
{
	return _attack_delay.size();
}

bool Monster::GetFacingDirection() const
{
	return _facing_direction;
}

bool Monster::IsAlive() const
{
	return _is_alive;
}

void Monster::SetStateString(std::string str)
{
	_state_string = str;
}

std::string Monster::GetStateString() const
{
	return _state_string;
}

void Monster::IsJumping()
{
	float outY = 0;
	FootHold* footHold;
	bool isFoothold = MapManager::GetInstance()->FootholdYCollisionMonster(this, &outY, &footHold);

	if (isFoothold)
	{
		if (_info.y <= outY)
		{
			if (_info.y - outY <= GetSpeed() && _info.y - outY >= -GetSpeed())
			{
				_info.y = outY;
			}
			else
			{
				_info.y = outY;
				//_info.y += GetSpeed();
			}
		}
		else
		{
			_info.y = outY;
		}
	}
}

bool Monster::IsChangeFoothold()
{
	float outY = 0;
	FootHold* footHold;
	if (_this_foothold == nullptr)
	{
		MapManager::GetInstance()->FootholdYCollisionMonster(this, &outY, &_this_foothold);
	}
	bool isFoothold = MapManager::GetInstance()->FootholdYCollisionMonster(this, &outY, &footHold);
	if (_this_foothold != footHold)
	{
		return true;
	}

	return false;
}

void Monster::AttackApply(const std::string key)
{
	if (_target == nullptr)
	{
		return;
	}
	RECT rc;
	AttackInfo* attack_info = _attack_info.find(key)->second;
	RECT range = attack_info->GetRange();
	range.left = static_cast<int>(_info.x) - std::abs(range.left);
	range.top = static_cast<int>(_info.y) - std::abs(range.top);
	range.right = static_cast<int>(_info.x) + std::abs(range.right);
	range.bottom = static_cast<int>(_info.y) + std::abs(range.bottom);
	const RECT target = _target->GetRect();

	const uint64_t tick = GetTickCount64();
	const auto player = dynamic_cast<Player*>(_target);

	auto bool1 = tick > static_cast<uint64_t>(attack_info->GetAttackAfterTick()) + attack_info->GetAttackAfter();
	auto bool2 = attack_info->GetAttackAfterTick() > 1;
	if(bool1 && bool2 && _is_attack)
	{
	std::cout << key << bool1 << " / " << bool2 << std::endl;
		
	}

	if (!player->IsDead())
	{
		if (_is_attack &&
			(attack_info->GetAttackAfterTick() > 1 &&
				tick > static_cast<uint64_t>(attack_info->GetAttackAfterTick()) + attack_info->GetAttackAfter()))
		{
			if (IntersectRect(&rc, &range, &target))
			{
				if (attack_info->IsDeadlyAttack())
				{
					if (player != nullptr)
					{
						player->SetHp(1);
						player->SetMp(1);
						player->SetInvincibility();
						player->SettingPushKnockBack(true);
						player->GetDamageHandler()->InsertTakeDamageEffect(player, 30000, 1000);
					}
				}
				else
				{
					player->SetInvincibility();
					player->SettingPushKnockBack(true);
					int damage = rand() %  this->GetPad() + (this->GetPad() / 2);
					player->GainHp(-damage);
					player->GetDamageHandler()->InsertTakeDamageEffect(player, damage, 1000);
				}

			}
			if (!player->IsInvincibility() && !attack_info->GetAreaAttack().empty())
			{
				for (const auto area : attack_info->GetAreaAttack())
				{
					const RECT range3 = attack_info->GetRange();
					RECT range2{

					area.x - std::abs(range3.left),
					area.y - std::abs(range3.top),
					area.x + std::abs(range3.right),
					area.y + std::abs(range3.bottom) };
					if (IntersectRect(&rc, &range2, &target))
					{
						player->SetInvincibility();
						player->SettingPushKnockBack(true);
						int damage = rand() % this->GetPad() + (this->GetPad() / 2);
						player->GainHp(-damage);
						player->GetDamageHandler()->InsertTakeDamageEffect(player, damage, 1000);
						int a = 0;
					}
				}
			}
			attack_info->SetAttackAfterTick(1);
			attack_info->SetEffectAfterTick(1);
			this->GainMp(-attack_info->GetConMp());
		}
	}

}

uint64_t Monster::GetAttackTick() const
{
	return _attack_tick;
}

MonsterFrameManager* Monster::GetBaseFrame() const
{
	return _base_state_frame;
}

void Monster::SetIsAttacking(bool g)
{
	_is_attack = g;
}

bool Monster::IsAttacking() const
{
	return _is_attack;
}

void Monster::SetAttackTick()
{
	_attack_tick = GetTickCount64();
}

int Monster::ReadyGameObject()
{

	HDC hDC = GetDC(_hWnd);
	_memDC = CreateCompatibleDC(hDC);
	_bitmap = CreateCompatibleBitmap(hDC, 800, 800);
	_old_bitmap = static_cast<HBITMAP>(SelectObject(_memDC, _bitmap));
	_memDC2 = CreateCompatibleDC(hDC);
	_bitmap2 = CreateCompatibleBitmap(hDC, 800, 800);
	_old_bitmap2 = static_cast<HBITMAP>(SelectObject(_memDC2, _bitmap2));
	_ai_movement = new AiMovement(GetSpeed());
	_ai_movement->SetPartner(this);
	ReleaseDC(_hWnd, hDC);
	_base_state_frame = new MonsterFrameManager();
	return 0;
}

void Monster::UpdateGameObject(const float deltaTime)
{
	if (_base_state_frame->GetFrameSize() == 0)
	{
		_base_state_frame->SetThisFrame(_this_frame);
	}
	UpdateRect();
	float movingVluae = 0;
	if (!IsAlive())
	{
		ChangeState(MonsterState::kDie);
		if (_state == State::kDead)
		{
			return;
		}

		if (!_is_drop_item)
		{
			DropDataManager::GetInstance()->DropFromMonster(this->GetMonsterId(), this);
			_is_drop_item = true;
		}

		if (_base_state_frame->GetFrameNumber() >= _base_state_frame->GetFrameSize())
		{
			_base_state_frame->SetFrameNumber(_base_state_frame->GetFrameNumber() - 1);
			 if (GetTickCount64() > _die_wait_tick + 250) //죽어도 시체가 남아있는 시간..
			{
				_state = State::kDead;
			}
			 return;
		}
		if (_die_wait_tick == 0) {
			_die_wait_tick = GetTickCount64();
		}
		return;
	}
	if (_monster_state != MonsterState::kHit)
	{
		movingVluae = _ai_movement->Moveing();
		if (movingVluae != 0.f)
		{
			ChangeState(MonsterState::kMove);
		}
		else
		{
			if (_monster_state == MonsterState::kStand ||
				_monster_state == MonsterState::kDie ||
				_monster_state == MonsterState::kHit)
			{
				ChangeState(MonsterState::kStand);
			}
		}
		if (_monster_state == MonsterState::kAttack1 || _monster_state == MonsterState::kAttack2
			 || _monster_state == MonsterState::kAttack3)
		{
			if (!this->GetStateString().empty())
			{
				this->AttackApply(this->GetStateString());
			}
		}
		float totalMoveX = 0;
		float totalMoveY = 0;
		float outY = 0;
		FootHold* footHold = nullptr;
		totalMoveX += movingVluae;

		if (/*strcmp(_monster_code.c_str(), "Client\\Mob\\8800000.img.xml") &&
			strcmp(_monster_code.c_str(), "Client\\Mob\\8800003.img.xml")&&
			strcmp(_monster_code.c_str(), "Client\\Mob\\8800004.img.xml")&&
			strcmp(_monster_code.c_str(), "Client\\Mob\\8800005.img.xml")&&
			strcmp(_monster_code.c_str(), "Client\\Mob\\8800006.img.xml")&&
			strcmp(_monster_code.c_str(), "Client\\Mob\\8800007.img.xml")&&
			strcmp(_monster_code.c_str(), "Client\\Mob\\8800008.img.xml")&&
			strcmp(_monster_code.c_str(), "Client\\Mob\\8800009.img.xml")&&
			strcmp(_monster_code.c_str(), "Client\\Mob\\8800010.img.xml")*/
			_monster_code.find("Mob\\88000") == std::string::npos)
		{
			_facing_direction = _ai_movement->GetFacingDirection();
		}
		if (totalMoveX != 0 || totalMoveY != 0)
		{
			_info.x += totalMoveX;
			_info.y += totalMoveY;
		}
		
		if (auto changeHold = IsChangeFoothold() || MapManager::GetInstance()->FootholdAndRectCollision(this))
		{
			_info.x -= totalMoveX;
			_info.y -= totalMoveY;
		}
	}
	else
	{
		int a = 1;
	}
	IsJumping();
}

void Monster::RenderGameObject(HDC hdc)
{
	UpdateRect();
	if (_this_frame.empty())
	{
		return;
	}
	const auto data = _this_frame[_base_state_frame->GetFrameNumber() % _this_frame.size()];
	const auto image = data->GetImage();

	const HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
	const HBRUSH brushPrev = static_cast<HBRUSH>(SelectObject(_memDC, brush));
	Rectangle(_memDC,
		0 - 10,
		0 - 10,
		static_cast<int>(image->GetWidth() + 10),
		static_cast<int>(image->GetHeight()) + 10);
	SelectObject(_memDC, brushPrev);
	DeleteObject(brush);
	DeleteObject(brushPrev);
	image->RenderBitmapImage(_memDC,
		0,
		0,
		static_cast<int>(image->GetWidth()),
		static_cast<int>(image->GetHeight()));

	if (GetFacingDirection())
	{
		StretchBlt(_memDC,
			0,
			0,
			image->GetWidth(),
			image->GetHeight(),
			_memDC,
			image->GetWidth() - 1,
			0,
			-image->GetWidth(),
			image->GetHeight(), SRCCOPY);
	}

	if (!IsAlive() && _die_wait_tick > 0)
	{
		BLENDFUNCTION bf{ 0,0 , _alpha_value, 0 };
		auto tick = GetTickCount64();
		if (tick > _alpha_tick + 40)
		{
			_alpha_value -= 15;
			_alpha_tick = tick;
		}

		BitBlt(_memDC2, 0, 0,
			image->GetWidth(),
			image->GetHeight(), 
			hdc,
			static_cast<int>(_rect.left + ScrollManager::GetScrollX()),
			static_cast<int>(_rect.bottom - image->GetHeight() + ScrollManager::GetScrollY()), SRCCOPY);

		GdiTransparentBlt(_memDC2,
			0,
			0,
			image->GetWidth(),
			image->GetHeight(),
			_memDC,
			0,
			0,
			image->GetWidth(),
			image->GetHeight(),
			RGB(255, 0, 255));


		GdiAlphaBlend(hdc,
			static_cast<int>((_info.x - data->GetOriginPos().x) + ScrollManager::GetScrollX()),
			static_cast<int>(_info.y - (data->GetOriginPos().y) + ScrollManager::GetScrollY()),
			image->GetWidth(),
			image->GetHeight(),
			_memDC2,
			0, 0,
			image->GetWidth(),
			image->GetHeight(), bf);
	}
	else
	{
		switch (_monster_state) {
		case MonsterState::kStand:
		case MonsterState::kMove:
		case MonsterState::kHit:
		case MonsterState::kDie:
		{
			GdiTransparentBlt(hdc,
				static_cast<int>((_info.x - data->GetOriginPos().x) + ScrollManager::GetScrollX()),
				static_cast<int>(_info.y - (data->GetOriginPos().y) + ScrollManager::GetScrollY()),
				image->GetWidth(),
				image->GetHeight(),
				_memDC,
				0,
				0,
				image->GetWidth(),
				image->GetHeight(),
				RGB(255, 0, 255));
			break;
		}
		case MonsterState::kAttack1:
		case MonsterState::kAttack2:
		case MonsterState::kAttack3:
		{
			auto reduceY = std::abs(static_cast<int>(_rect.bottom - data->GetOriginPos().y + ScrollManager::GetScrollY()));
			GdiTransparentBlt(hdc,
				static_cast<int>(_info.x - (data->GetOriginPos().x) + ScrollManager::GetScrollX()),
				static_cast<int>(_info.y - (data->GetOriginPos().y) + ScrollManager::GetScrollY()),
				image->GetWidth(),
				image->GetHeight(),
				_memDC,
				0,
				0,
				image->GetWidth(),
				image->GetHeight(),
				RGB(255, 0, 255));

			auto attack_info = _attack_info.find(_state_string);
			if (attack_info != _attack_info.end())
			{
				if (attack_info->second->GetAttackAfter() > 0)
				{
					if (attack_info->second->GetEffectAfterTick() == 0)
					{
						attack_info->second->SetEffectAfterTick(GetTickCount64());
					}
					if (attack_info->second->GetAttackAfterTick() == 0)
					{
						attack_info->second->SetAttackAfterTick(GetTickCount64());
					}
				}
				if (!attack_info->second->GetEffect().empty())
				{
					const auto effect_list = attack_info->second->GetEffect();
					const int64_t tick = GetTickCount64();
					if (tick > attack_info->second->GetEffectAfterTick() + attack_info->second->GetEffectAfter() && 
						!attack_info->second->IsEffectFinish())
					{
						if (attack_info->second->GetEffectFrame()->GetFrameNumber() < effect_list.size())
						{
							const auto effect_shared_image = effect_list[attack_info->second->GetEffectFrame()->GetFrameNumber() % effect_list.size()];
							const auto effect_image = effect_shared_image->GetImage();
							effect_image->RenderBitmapImage(hdc,
								static_cast<int>((_info.x - effect_shared_image->GetOriginPos().x) + ScrollManager::GetScrollX()),
								static_cast<int>(_info.y - (effect_shared_image->GetOriginPos().y) + ScrollManager::GetScrollY()),
								effect_image->GetWidth(),
								effect_image->GetHeight());
						}
					}
				}
				if (!attack_info->second->GetAreaWarning().empty())
				{
					const auto effect_list = attack_info->second->GetAreaWarning();
					for (auto area : attack_info->second->GetAreaAttack())
					{
						const int64_t tick = GetTickCount64();
						if (attack_info->second->GetAreaWarningFrame()->GetFrameNumber() < effect_list.size())
						{
							const auto effect_shared_image = effect_list[attack_info->second->GetAreaWarningFrame()->GetFrameNumber() % effect_list.size()];
							const auto effect_image = effect_shared_image->GetImage();
							effect_image->RenderBitmapImage(hdc,
								static_cast<int>((area.x - effect_shared_image->GetOriginPos().x) + ScrollManager::GetScrollX()),
								static_cast<int>(_info.y - (effect_shared_image->GetOriginPos().y) + ScrollManager::GetScrollY()),
								effect_image->GetWidth(),
								effect_image->GetHeight());
						}
					}
				}
				if (!attack_info->second->GetHitEffect().empty())
				{
					/*	const auto effect_list = _attack_info->GetHitEffect();
						const auto effect_shared_image = effect_list[_attack_info->GetHitEffectFrame()->GetFrameNumber() % effect_list.size()];
						const auto effect_image = (*effect_shared_image)->GetImage();
						(*effect_image)->RenderBitmapImage(hdc,
							static_cast<int>((_info.x - (*effect_shared_image)->GetOriginPos().x) + ScrollManager::GetScrollX()),
							static_cast<int>(_rect.bottom - (*effect_shared_image)->GetOriginPos().y + ScrollManager::GetScrollY()),
							(*effect_image)->GetWidth(),
							(*effect_image)->GetHeight());*/
				}
			}

		}
		break;
		default:;
		}
		//Rectangle(hdc, 
		//	_rect.left + static_cast<int>(ScrollManager::GetScrollX()), 
		//	_rect.top + static_cast<int>(ScrollManager::GetScrollY()),
		//	_rect.right + static_cast<int>(ScrollManager::GetScrollX()),
		//	_rect.bottom + static_cast<int>(ScrollManager::GetScrollY()));
	}
}

void Monster::LateUpdateGameObject()
{
	switch (_monster_state) {
	case MonsterState::kStand:
	case MonsterState::kMove:
	case MonsterState::kHit:
	case MonsterState::kDie:
		break;
	case MonsterState::kAttack1:
	case MonsterState::kAttack2: 
	case MonsterState::kAttack3:
	{
		const auto attack_info = _attack_info.find(_state_string);
		if (attack_info != _attack_info.end())
		{
			if (!attack_info->second->GetEffect().empty())
			{
				const int64_t tick = GetTickCount64();
				//if (tick > attack_info->second->GetAttackAfterTick() + attack_info->second->GetAttackAfter())
				//{
					if (attack_info->second->GetEffectFrame()->NextFrame(false))
					{
						attack_info->second->SetEffectFinish(true);
					}
				//}
			}
			else
			{
				attack_info->second->SetEffectFinish(true);
			}
			if (!attack_info->second->GetAreaWarning().empty())
			{
				if (!attack_info->second->GetAreaAttack().empty())
				{
					if (attack_info->second->GetAreaWarningFrame()->NextFrame(false))
					{
						attack_info->second->SetAttackFinish(true);
					}
				}
				else
				{
					attack_info->second->SetAttackFinish(true);
				}
			}
			else
			{
				attack_info->second->SetAttackFinish(true);
			}
			if (!attack_info->second->GetHitEffect().empty())
			{
				attack_info->second->GetHitEffectFrame()->NextFrame(false);
			}

			if (attack_info->second->IsEffectFinish() && attack_info->second->IsAttackFinish() && _base_state_frame->IsFrameFinish())
			{
				attack_info->second->ResetEffectFrame();
				attack_info->second->ResetAreaFrame();
				this->SetIsAttacking(false);
				_base_state_frame->ResetFrame();
				ChangeState(MonsterState::kStand);
				return;
			}
		}

		break;
	}
	default:;
	}

		if (_monster_state == MonsterState::kHit)
		{
			if (_base_state_frame->NextFrame(false))
			{
				ChangeState(MonsterState::kStand);
			}
		}
		else if (_monster_state == MonsterState::kMove || _monster_state == MonsterState::kStand)
		{
			if (_base_state_frame->NextFrame())
			{
				ChangeState(MonsterState::kStand);
			}
		}
		else if (_monster_state == MonsterState::kDie)
		{
			if (_base_state_frame->NextFrame(false))
			{
				
			}
		}
		else
		{
			if (_base_state_frame->NextFrame(false))
			{
			}
		}
}

void Monster::UpdateRect()
{
	const auto data = _this_frame[_base_state_frame->GetFrameNumber() % _this_frame.size()];
	//RECT rect = (*data)->GetRect();
	_rect.left = static_cast<int>(_info.x + (*data).GetRect().left);
	_rect.top = static_cast<int>(_info.y + (*data).GetRect().top);
	_rect.right = static_cast<int>(_info.x +(*data).GetRect().right);
	_rect.bottom = static_cast<int>(_info.y +(*data).GetRect().bottom);
	_info.cx = data->GetOriginPos().x;
	_info.cy = data->GetOriginPos().y;
}

void Monster::ChangeState(MonsterState state)
{
	if (_monster_state != state)
	{
		_monster_state = state;
		switch (state)
		{
		case Monster::MonsterState::kStand:
		{
			_this_frame = _movement->FindMovement("stand");
			break;
		}
		case Monster::MonsterState::kMove:
			_this_frame = _movement->FindMovement("move");
			break;
		case Monster::MonsterState::kHit:
			_this_frame = _movement->FindMovement("hit1");
			if (_attack_info.find(_state_string) != _attack_info.end())
			{
				_attack_info.find(_state_string)->second->ResetEffectFrame();
				_attack_info.find(_state_string)->second->ResetAreaFrame();
			}
			break;
		case Monster::MonsterState::kAttack1:
			_this_frame = _movement->FindMovement("attack1");
			_state_string.clear();
			_state_string.append("attack1");
			break;
		case Monster::MonsterState::kAttack2:
			_this_frame = _movement->FindMovement("attack2");
			_state_string.clear();
			_state_string.append("attack2");
			break;
		case Monster::MonsterState::kAttack3:
			_this_frame = _movement->FindMovement("attack3");
			_state_string.clear();
			_state_string.append("attack3");
			break;
		case Monster::MonsterState::kDie:
			_this_frame = _movement->FindMovement("die1");
			break;
		default:
			break;
		}
		_base_state_frame->ResetFrame();
		_base_state_frame->SetThisFrame(_this_frame);
		if(!_attack_info.empty())
		{
			if (_monster_state == MonsterState::kAttack1 || 
				_monster_state == MonsterState::kAttack2 || 
				_monster_state == MonsterState::kAttack3)
			{
				auto attack_info = _attack_info.find(_state_string);
				if (attack_info != _attack_info.end())
				{
					if (!attack_info->second->GetEffect().empty())
					{
						attack_info->second->GetEffectFrame()->SetThisFrame(attack_info->second->GetEffect());
					}
					if (!attack_info->second->GetHitEffect().empty())
					{
						attack_info->second->GetHitEffectFrame()->SetThisFrame(attack_info->second->GetHitEffect());
					}
					if (!attack_info->second->GetAreaWarning().empty())
					{
						attack_info->second->GetAreaWarningFrame()->SetThisFrame(attack_info->second->GetAreaWarning());
						
						if (attack_info->second->GetAreaAttack().empty())
						{
							attack_info->second->InsertAreaAttack({ static_cast<long>(_info.x) + -400, static_cast<long>((_this_foothold->GetStartPos().y)) });
							attack_info->second->InsertAreaAttack({ static_cast<long>(_info.x) + -200 , static_cast<long>(_this_foothold->GetStartPos().y) });
							attack_info->second->InsertAreaAttack({ static_cast<long>(_info.x), static_cast<long>(_this_foothold->GetStartPos().y) });
							attack_info->second->InsertAreaAttack({ static_cast<long>(_info.x) + 200, static_cast<long>(_this_foothold->GetStartPos().y) });
							attack_info->second->InsertAreaAttack({ static_cast<long>(_info.x) + 400, static_cast<long>(_this_foothold->GetStartPos().y) });

						}
					}
				}
			}
		}
	}
}
