#pragma once
#include "../Base/game_object.h"

class SkinInfo;
class SkinItem;
class SkinFrame;
class FootHold;
class Player :
    public GameObject
{
public:
    Player(uint8_t layer = 5);
    ~Player();


    virtual int ReadyGameObject() override;
    virtual void UpdateGameObject(const float deltaTime) override;
    virtual void RenderGameObject(HDC hdc) override;
    virtual void LateUpdateGameObject() override;

    void ScrollMove();

public:

    void SetFrameThis(SkinFrame* frame);
    SkinFrame* GetFrameThis();

    std::vector<SkinInfo*> FindSkinFrame() const;
    void InsertFrame(SkinInfo* item);

    void ChangeFrameState(std::string frame);
    const char* GetFrameState() const;

    uint16_t GetFrameNummber(uint16_t remain);

    void SetFacingDirection(uint8_t direction);
    uint8_t GetFacingDirection() const;

    FootHold* GetNowFootHold();
    void SetNowFootHold(FootHold* hold);

    ObjectInfo* GetPlayerInfo();

private:
    void LoadCharacterFrame(std::string frameName, uint16_t frameCount = 3);
    void RenderCharacter(HDC hdc);

    void IsJumping();
private:
    uint16_t _skin_id;
    uint16_t _frame_nummber;
    size_t _this_frame_max_count;
    bool _frame_revers;
    bool _is_prone;
    bool _is_jump;
    bool _is_fly;
    bool _is_rope;
    uint16_t _jump_count;
    uint8_t _facing_direction;
    SkinFrame* _frame_this;
    std::string _frame_state;
    std::map<std::string, SkinItem*> _skin_frames;
    std::map<std::string, SkinItem*> _head_skin_frames;
    uint64_t _frame_tick;

    ObjectInfo _player_info;


    FootHold* _now_foothold;
    FootHold* _next_foothold;
    bool _is_first_foothold;

    float _jump_power = 15.f;
    float _accel = 0.f;
    float _gravity = 9.f;

    HDC _memDC;
    HBITMAP _bitmap;
    HBITMAP _old_bitmap;
};