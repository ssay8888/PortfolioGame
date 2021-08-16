#pragma once
typedef struct {
	float x;
	float y;
	int cx;
	int cy;
} Info;

typedef struct {
	float x;
	float y;
} ObjectPos;

typedef struct {
	uint8_t level;
	int16_t max_hp;
	int16_t eqp_max_hp;
	int16_t hp;
	int16_t max_mp;
	int16_t eqp_max_mp;
	int16_t mp;
	int16_t ap;
	int16_t sp;
	int32_t exp;
	int16_t job;
	int16_t str;
	int16_t dex;
	int16_t int_;
	int16_t luk;
	int16_t eqp_str;
	int16_t eqp_dex;
	int16_t eqp_int_;
	int16_t eqp_luk;
	int16_t eqp_pad;
	int16_t eqp_mad;
	int16_t eqp_pdd;
	int16_t eqp_mdd;
	uint32_t min_power;
	uint32_t max_power;
	std::string name;
} ObjectInfo;

typedef struct {
	uint8_t level;
	uint8_t undead;
	uint8_t body_attack;
	uint16_t pad;
	uint16_t mad;
	uint16_t pdd;
	uint16_t mdd;
	float speed;
	uint32_t exp;
	int32_t hp;
	int32_t max_hp;
	int32_t mp;
	int32_t max_mp;
	std::string name;
} MonsterInfo;