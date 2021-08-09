#include "../../pch.h"
#include "monster.h"
#include "../Base/game_object.h"
#include "../../Managers/MonsterMnager/monster_manager.h"
#include "../../Managers/MonsterMnager/monster_movement.h"
#include "../../Managers/MonsterMnager/monster_parts.h"
#include "../../Managers/ScrollManager/scroll_manager.h"
#include "../../Managers/MapManager/map_manager.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"

Monster::Monster() :
    GameObject(0),
    _monster_info({}),
    _now_foothold(nullptr),
    _facing_direction(false),
    _frame_nummber(0),
    _frame_revers(false),
    _frame_tick(0),
    _monster_state(MonsterState::kStand),
    _bitmap(nullptr),
    _old_bitmap(nullptr),
    _memDC(nullptr)


{
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
        _state = State::kDead;
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
        _state = State::kDead;
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

void Monster::SetMaxMp(uint32_t maxmp)
{
    _monster_info.max_mp = maxmp;
}

uint32_t Monster::GetMaxMp() const
{
    return _monster_info.max_mp;
}

void Monster::SetMovement(std::shared_ptr<MonsterMovement*> movement)
{
    _movement = movement;
    _this_frame = (*_movement)->FindMovement("stand");
}

std::shared_ptr<MonsterMovement*> Monster::GetMovement()
{
    return _movement;
}

void Monster::SetName(std::string name)
{
    _monster_info.name = name;
}

std::string Monster::GetName() const
{
    return _monster_info.name;
}

bool Monster::GetFacingDirection()
{
    return _facing_direction;
}

bool Monster::IsAlive() const
{
    return _is_alive;
}

void Monster::IsJumping()
{
    //float outY = 0;
    //FootHold* footHold;
    //bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &footHold);

	/*if (isFoothold)
	{
		if (_info.y <= outY)
		{
			if (_info.y - outY <= GetSpeed() && _info.y - outY >= -GetSpeed())
			{
				_info.y = outY;
			}
			else
			{
				_info.y += GetSpeed();
			}
		}
		else
		{
			_info.y = outY;
		}
    }*/
}

int Monster::ReadyGameObject()
{

    HDC hDC = GetDC(_hWnd);
    _memDC = CreateCompatibleDC(hDC);
    _bitmap = CreateCompatibleBitmap(hDC, 1024, 768);
    _old_bitmap = (HBITMAP)SelectObject(_memDC, _bitmap);
    ReleaseDC(_hWnd, hDC);
    return 0;
}

void Monster::UpdateGameObject(const float deltaTime)
{
    ChangeState(MonsterState::kMove);
    float totalMoveX = 0;
    float totalMoveY = 0;
    float outY = 0;
    FootHold* footHold = nullptr;
    totalMoveX -= GetSpeed();


    if (totalMoveX != 0 || totalMoveY != 0)
    {
        if (totalMoveX > 0)
        {
            _facing_direction = 1;
        }
        else
        {
            _facing_direction = 0;
        }
        //if (_now_foothold == nullptr)
        //{
        //    //처음 발판을 설정.
        //    MapManager::GetInstance()->FootholdYCollision(this, &outY, &_now_foothold);
        //}
        //_info.x += totalMoveX;
        //_info.y += totalMoveY;
        //FootHold* hold;
        //if (MapManager::GetInstance()->FootholdYCollision(this, &outY, &hold))
        //{
        //    if (hold != _now_foothold) // 발판이 바뀌면 되돌아감.
        //    {
        //        _info.x -= totalMoveX;
        //        _info.y -= totalMoveY;
        //    }
        //}
    }



    IsJumping();
    uint64_t tick = GetTickCount64();
    if (tick > _frame_tick + (*_this_frame[_frame_nummber % _this_frame.size()])->GetDelay())
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
            if (++_frame_nummber >= _this_frame.size() - 1)
            {
                _frame_revers = true;
            }
        }
        _frame_tick = tick;
    }
}

void Monster::RenderGameObject(HDC hdc)
{
    UpdateRectGameObject();
    auto data = _this_frame[_frame_nummber % _this_frame.size()];
    auto image = (*data)->GetImage();
    Rectangle(_memDC,
        0,
        0,
        static_cast<int>((*image)->GetWidth()),
        static_cast<int>((*image)->GetHeight()));
    (*image)->RenderBitmapImage(_memDC,
        0,
        0,
        static_cast<int>((*image)->GetWidth()),
        static_cast<int>((*image)->GetHeight()));
    if (GetFacingDirection())
    {
        StretchBlt(_memDC,
            0,
            0,
            (*image)->GetWidth(), 
            (*image)->GetHeight(), 
            _memDC, 
            (*image)->GetWidth() - 1,
            0, 
            -(*image)->GetWidth(),
            (*image)->GetHeight(), SRCCOPY);
    }

    Rectangle(hdc,
        static_cast<int>(_rect.left + ScrollManager::GetScrollX()),
        static_cast<int>(_rect.top + ScrollManager::GetScrollY()),
            static_cast<int>(_rect.right + ScrollManager::GetScrollX()),
                static_cast<int>(_rect.bottom + ScrollManager::GetScrollY()));

    GdiTransparentBlt(hdc,
        static_cast<int>(_rect.left + ScrollManager::GetScrollX()),
        static_cast<int>(_rect.top + ScrollManager::GetScrollY()),
        (*image)->GetWidth(),
        (*image)->GetHeight(),
        _memDC,
        0, 0,
        (*image)->GetWidth(),
        (*image)->GetHeight(),
        RGB(255, 0, 255));
}

void Monster::LateUpdateGameObject()
{
}

void Monster::ChangeState(MonsterState state)
{
    if (_monster_state != state)
    {
        _frame_tick = 0;
        _frame_nummber = 0;
        _monster_state = state;
        switch (state)
        {
        case Monster::MonsterState::kStand:
        {
            _this_frame = (*_movement)->FindMovement("stand");
            break;
        }
        case Monster::MonsterState::kMove:
            _this_frame = (*_movement)->FindMovement("move");
            break;
        case Monster::MonsterState::kDie:
            _this_frame = (*_movement)->FindMovement("die1");
            break;
        default:
            break;
        }
    }
}
