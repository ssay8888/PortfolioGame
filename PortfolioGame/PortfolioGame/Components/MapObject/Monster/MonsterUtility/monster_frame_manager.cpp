#include "../../../../pch.h"
#include "monster_frame_manager.h"
#include "../../../../Managers/MonsterMnager/monster_parts.h"

MonsterFrameManager::MonsterFrameManager(): _frame_tick(0),
                                            _frame_number(0),
                                            _frame_revers(false),
                                            _frame_finish(false)
{
}

MonsterFrameManager::~MonsterFrameManager() = default;

uint64_t MonsterFrameManager::GetFrameTick() const
{
    return _frame_tick;
}

uint64_t MonsterFrameManager::GetFrameNumber() const
{
    return _frame_number;
}

bool MonsterFrameManager::IsFrameRevers() const
{
    return _frame_revers;
}

void MonsterFrameManager::SetFrameTick(const uint64_t tick)
{
    _frame_tick = tick;
}

void MonsterFrameManager::SetFrameNumber(const uint16_t number)
{
    _frame_number = number;
}

void MonsterFrameManager::SetFrameRevers(const bool revers)
{
    _frame_revers = revers;
}

void MonsterFrameManager::SetThisFrame(std::vector<std::shared_ptr<MonsterParts*>> frame)
{
    _this_frame = frame;
}

void MonsterFrameManager::ResetFrame()
{
    _frame_tick = 0;
    _frame_number = 0;
    _frame_revers = false;
    _frame_finish = false;
}

bool MonsterFrameManager::NextFrame(bool revers)
{
    if (_this_frame.empty())
    {
        return true;
    }
    uint64_t tick = GetTickCount64();
    if (tick > _frame_tick + (*_this_frame[_frame_number % _this_frame.size()])->GetDelay())
    {

        if (_frame_revers)
        {
            if (revers)
            {
                if (_frame_number-- == 0)
                {
                    _frame_number = 0;
                    _frame_revers = false;
                }
            }
        }
        else
        {
            if (_frame_number++ >= _this_frame.size())
            {
                _frame_revers = true;
                _frame_finish = true;
            }
        }
        _frame_tick = tick;
    }
    // _info.cy = (*(*_this_frame[_frame_nummber % _this_frame.size()])->GetImage())->GetHeight();
    return _frame_revers; // 리버스가 true되면 모션이 끝난것으로 간주!
}

bool MonsterFrameManager::IsFrameFinish() const
{
    return _frame_finish;
}

size_t MonsterFrameManager::GetFrameSize() const
{
    return _this_frame.size();
}
