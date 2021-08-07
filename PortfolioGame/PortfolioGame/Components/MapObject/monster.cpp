#include "../../pch.h"
#include "monster.h"
#include "../../Managers/MonsterMnager/monster_manager.h"
#include "../../Managers/MonsterMnager/monster_movement.h"
#include "../../Managers/MonsterMnager/monster_parts.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"

Monster::Monster() :
    GameObject(0),
    _monster_info({})
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

void Monster::SetSpeed(uint8_t speed)
{
    _monster_info.speed = speed;
}

uint8_t Monster::GetSpeed() const
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
}

uint32_t Monster::GetHp() const
{
    return _monster_info.hp;
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

int Monster::ReadyGameObject()
{
    return 0;
}

void Monster::UpdateGameObject(const float deltaTime)
{
    ChangeState(MonsterState::kMove);
    uint64_t tick = GetTickCount64();
    if (tick > _frame_tick + (*_this_frame[_frame_nummber % _this_frame.size()])->GetDelay())
    {
        _frame_nummber++;
        if (_frame_nummber >= _this_frame.size())
        {
            _frame_nummber = 0;
        }
        _frame_tick = tick;
    }
}

void Monster::RenderGameObject(HDC hdc)
{
    auto data = _this_frame[_frame_nummber % _this_frame.size()];
    auto image = (*data)->GetImage();
    (*image)->RenderBitmapImage(hdc, 
        static_cast<int>(_info.x), 
        static_cast<int>(_info.y),
        static_cast<int>((*image)->GetWidth()),
        static_cast<int>((*image)->GetHeight()));
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
            break;
        default:
            break;
        }
    }
}
