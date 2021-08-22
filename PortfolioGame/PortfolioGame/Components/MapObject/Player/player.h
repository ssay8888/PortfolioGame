#pragma once
#include "../../Base/game_object.h"

class Item;
class BuffStat;
class Npc;
class QuestInfo;
class Equipment;
class EqpInventory;
class Inventory;
class MagicAttack;
class Skill;
class SkinInfo;
class SkinItem;
class SkinFrame;
class FootHold;
class Monster;
class DamageHandler;
class Player :
    public GameObject
{
public:
    Player(uint8_t layer = 6);
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

    FootHold* GetNowFootHold() const;
    void SetNowFootHold(FootHold* hold);

    ObjectInfo* GetPlayerInfo();
    void SetDead(bool isdead);
    bool IsDead();

    int16_t GetHp() const;
    int16_t GetMaxHp() const;
    int16_t GetEqpMaxHp() const;
    int16_t GetTotalMaxHp() const;
    int16_t GetMp() const;
    int16_t GetMaxMp() const;
    int16_t GetEqpMaxMp() const;
    int16_t GetTotalMaxMp() const;
    int16_t GetAp() const;
    int16_t GetSp() const;
    int16_t GetJob() const;
    int16_t GetLevel() const;
    int32_t GetExp() const;
    int16_t GetStr() const;
    int16_t GetDex() const;
    int16_t GetInt() const;
    int16_t GetLuk() const;
    int16_t GetEqpStr() const;
    int16_t GetEqpDex() const;
    int16_t GetEqpInt() const;
    int16_t GetEqpLuk() const;
    int16_t GetTotalStr() const;
    int16_t GetTotalDex() const;
    int16_t GetTotalInt() const;
    int16_t GetTotalMad() const;
    int16_t GetTotalLuk() const;
    int16_t GetEqpPad() const;
    int16_t GetEqpMad() const;
    int16_t GetEqpPdd() const;
    int16_t GetEqpMdd() const;
    uint32_t GetMinPower() const;
    uint32_t GetMaxPower() const;
    int32_t GetMeso() const;
    std::string GetName() const;
    BuffStat* GetBuffStat();
    std::shared_ptr<QuestInfo> FindClearQuest(int32_t npc_id);
    void InsertClearQuest(std::shared_ptr<QuestInfo> info);
    void RemoveClearQuest(std::shared_ptr<QuestInfo> info);
    std::shared_ptr<QuestInfo> FindIngQuest(int32_t npc_id);
    void InsertIngQuest(std::shared_ptr<QuestInfo> info);
    void RemoveIngQuest(std::shared_ptr<QuestInfo> info);

    void GainHp(int16_t value);
    void GainEqpMaxHp(int16_t value);
    void GainMaxHp(int16_t value);
    void GainMp(int16_t value);
    void GainEqpMaxMp(int16_t value);
    void GainMaxMp(int16_t value);
    void SetHp(int16_t value);
    void SetMaxHp(int16_t value);
    void SetMp(int16_t value );
    void SetMaxMp(int16_t value);
    void GainAp(int16_t value);
    void GainSp(int16_t value);
    void GainStr(int16_t value);
    void GainDex(int16_t value);
    void GainInt(int16_t value);
    void GainLuk(int16_t value);
    void SetJob(int16_t value);
    void GainLevel(int16_t value);
    void GainExp(int32_t value, bool level_up = true);
    void GainEqpStr(int16_t value);
    void GainEqpDex(int16_t value);
    void GainEqpInt(int16_t value);
    void GainEqpLuk(int16_t value);
    void GainEqpPad(int16_t value);
    void GainEqpMad(int16_t value);
    void GainEqpPdd(int16_t value);
    void GainEqpMdd(int16_t value);
    void GainMinPower(uint32_t value);
    void GainMaxPower(uint32_t value);
    void GainMeso(uint32_t value);

    bool IsInvincibility();
    void SetInvincibility();
    void ResetEqpStat();
    void RecalcEqpStat();

    DamageHandler* GetDamageHandler() const;
    void SettingPushKnockBack(bool fancing);

    Inventory* GetInventory(::ObjectType::InventoryTabState type);
    EqpInventory* GetEqpInventory();

    Equipment* GetEquipment();
    void LevelUp();
private:
    void LoadCharacterFrame(std::string frameName, uint16_t frameCount = 3);
    void RenderCharacter(HDC hdc);

    void IsJumping();
    void TryMeleeAttack();

    bool IsAlertStateTick();
    void UpdateAlertTick();
    bool IsFlickerStateTick() const;
    void UpdateFlickerTick();
    void AttackMonster(Monster* monster);
    void TakeDamage();

    void ApplySkill();
private:
    uint16_t _skin_id;
    uint16_t _frame_nummber;
    size_t _this_frame_max_count;
    bool _frame_revers;
    bool _is_prone;
    bool _is_jump;
    bool _is_fly;
    bool _is_rope;
    bool _is_dead;
    uint16_t _jump_count;
    uint8_t _facing_direction;
    SkinFrame* _frame_this;
    std::string _frame_state;
    std::map<std::string, SkinItem*> _skin_frames;
    std::map<std::string, SkinItem*> _head_skin_frames;
    uint64_t _frame_tick;

    ObjectInfo _player_info;

    /// <summary>
    /// 공격관련
    /// </summary>
    RECT _melee_attack_hitbox;
    RECT _magic_attack_hitbox;
    bool _is_attacking;
    uint64_t _alert_tick;
    uint64_t _flicker_tick;

    MagicAttack* _attack_skill;

    /// <summary>
    /// 버프관련
    /// </summary>
    BuffStat* _buffstat;

    

    ///피격관련
    uint64_t _take_damage_tick;
    uint64_t _knockback_tick;
    bool _knockback_facing_direction;
    const uint32_t _knockback_time = 200;


    /// <summary>
    /// 발판관련
    /// </summary>
    FootHold* _now_foothold;
    FootHold* _next_foothold;
    bool _is_first_foothold;

    float _jump_power = 15.f;
    float _accel = 0.f;
    float _gravity = 9.f;
    ObjectPos _oldOrigin;

    /// <summary>
    /// 데미지관련
    /// </summary>
    DamageHandler* _damage_handler;

    /// <summary>
    /// 인벤토리관련
    /// </summary>
    Inventory* _inventory[::ObjectType::InventoryTabState::kEnd];
    EqpInventory* _eqp_inventory;
    Equipment* _equipment;
    uint64_t _use_item_tick;

    uint64_t _portal_tick;

    /// <summary>
    /// 퀘스트 관련
    /// </summary>
    std::map<int32_t, std::shared_ptr<QuestInfo>> _clear_quest_list;
    std::map<int32_t, std::shared_ptr<QuestInfo>> _ing_quest_list;
    std::shared_ptr<QuestInfo> _select_npc;

    /// <summary>
    /// 기타
    /// </summary>
	
    std::list<std::pair<POINT, std::shared_ptr<Item>>> _list_pickup;
};