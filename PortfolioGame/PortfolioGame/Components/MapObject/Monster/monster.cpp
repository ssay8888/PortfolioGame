#include "../../../pch.h"
#include "monster.h"
#include "AiMovement/ai_movement.h"
#include "../../../Managers/MonsterMnager/monster_manager.h"
#include "../../../Managers/MonsterMnager/monster_movement.h"
#include "../../../Managers/MonsterMnager/monster_parts.h"
#include "../../../Managers/ScrollManager/scroll_manager.h"
#include "../../../Managers/MapManager/map_manager.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"

Monster::Monster() :
    GameObject(0),
    _monster_info({}),
    _now_foothold(nullptr),
    _facing_direction(false),
    _frame_nummber(0),
    _frame_revers(false),
    _frame_tick(0),
    _is_alive(true),
    _monster_state(MonsterState::kStand),
    _die_wait_tick(0),
    _alpha_value(255),
    _ai_movement(nullptr),
    _alpha_tick(0),
    _this_foothold(nullptr),
    _bitmap2(nullptr),
    _old_bitmap2(nullptr),
    _memDC2(nullptr)


{
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
    float outY = 0;
    FootHold* footHold;
    bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &footHold);

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
				_info.y += GetSpeed();
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
        MapManager::GetInstance()->FootholdYCollision(this, &outY, &_this_foothold);
    }
    bool isFoothold = MapManager::GetInstance()->FootholdYCollision(this, &outY, &footHold);
    if (_this_foothold != footHold)
    {
        return true;
    }

    return false;
}

int Monster::ReadyGameObject()
{

    HDC hDC = GetDC(_hWnd);
    _memDC = CreateCompatibleDC(hDC);
    _bitmap = CreateCompatibleBitmap(hDC, 300, 300);
    _old_bitmap = (HBITMAP)SelectObject(_memDC, _bitmap);
    _memDC2 = CreateCompatibleDC(hDC);
    _bitmap2 = CreateCompatibleBitmap(hDC, 300, 300);
    _old_bitmap2 = (HBITMAP)SelectObject(_memDC2, _bitmap2);
    _ai_movement = new AiMovement(GetSpeed());
    ReleaseDC(_hWnd, hDC);
    return 0;
}

void Monster::UpdateGameObject(const float deltaTime)
{
    if (!IsAlive())
    {
        ChangeState(MonsterState::kDie);
        if (_state == State::kDead)
        {
            return;
        }
        auto endFrame = NextFrame();
        if (_die_wait_tick == 0) {
            if (endFrame)
            {
                _die_wait_tick = GetTickCount64();
            }
        }
        else if (GetTickCount64() > _die_wait_tick + 250) //죽어도 시체가 남아있는 시간..
        {
            _state = State::kDead;
        }
        return;
    }
    if (_ai_movement->Moveing() != 0)
    {
        ChangeState(MonsterState::kMove);
    }
    else
    {
        ChangeState(MonsterState::kStand);
    }
    float totalMoveX = 0;
    float totalMoveY = 0;
    float outY = 0;
    FootHold* footHold = nullptr;
    totalMoveX += _ai_movement->Moveing();


    if (totalMoveX != 0 || totalMoveY != 0)
    {
        _facing_direction = _ai_movement->GetFacingDirection();
        _info.x += totalMoveX;
        _info.y += totalMoveY;
    }

    UpdateRectGameObject();
    if (auto changeHold = IsChangeFoothold() || MapManager::GetInstance()->FootholdAndRectCollision(this))
    {
        _info.x -= totalMoveX;
        _info.y -= totalMoveY;
    }
    IsJumping();
    NextFrame();
}

void Monster::RenderGameObject(HDC hdc)
{
    UpdateRectGameObject();
    auto data = _this_frame[_frame_nummber % _this_frame.size()];
    auto image = (*data)->GetImage();

    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
    HBRUSH brushPrev = (HBRUSH)SelectObject(_memDC, brush);
    Rectangle(_memDC,
        0-10,
        0 - 10,
        static_cast<int>((*image)->GetWidth() + 10),
        static_cast<int>((*image)->GetHeight()) + 10);
    SelectObject(_memDC, brushPrev);
    DeleteObject(brush);
    DeleteObject(brushPrev);
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

    if (!IsAlive() && _die_wait_tick > 0)
    {
        BLENDFUNCTION bf{ 0,0 , _alpha_value, 0 };
        auto tick =  GetTickCount64();
        if (tick > _alpha_tick + 40)
        {
            _alpha_value -= 30;
            _alpha_tick = tick;
        }

        BitBlt(_memDC2, 0, 0,
            (*image)->GetWidth(),
            (*image)->GetHeight(), hdc,
            static_cast<int>(_rect.left + ScrollManager::GetScrollX()),
            static_cast<int>(_rect.bottom - ((*image)->GetHeight()) + ScrollManager::GetScrollY()), SRCCOPY);

        GdiTransparentBlt(_memDC2,
            0,
            0,
            (*image)->GetWidth(),
            (*image)->GetHeight(),
            _memDC,
            0,
            0,
            (*image)->GetWidth(),
            (*image)->GetHeight(),
            RGB(255, 0, 255));


        GdiAlphaBlend(hdc,
            static_cast<int>(_rect.left + ScrollManager::GetScrollX()),
            static_cast<int>(_rect.bottom - ((*image)->GetHeight()) + ScrollManager::GetScrollY()),
            (*image)->GetWidth(),
            (*image)->GetHeight(),
            _memDC2,
            0, 0,
            (*image)->GetWidth(),
            (*image)->GetHeight(), bf);
    }
    else
    {
        GdiTransparentBlt(hdc,
            static_cast<int>(_rect.left + ScrollManager::GetScrollX()),
            static_cast<int>(_rect.bottom - ((*image)->GetHeight()) + ScrollManager::GetScrollY()),
            (*image)->GetWidth(),
            (*image)->GetHeight(),
            _memDC,
            0,
            0,
            (*image)->GetWidth(),
            (*image)->GetHeight(),
            RGB(255, 0, 255));

    }
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
        _frame_revers = false;
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

void Monster::ShowDieMotion()
{
}

bool Monster::NextFrame()
{
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
   // _info.cy = (*(*_this_frame[_frame_nummber % _this_frame.size()])->GetImage())->GetHeight();
    return _frame_revers; // 리버스가 true되면 모션이 끝난것으로 간주!
}
