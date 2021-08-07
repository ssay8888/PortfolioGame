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
	int16_t hp;
	int16_t max_mp;
	int16_t mp;
	int32_t exp;
	std::string name;
} ObjectInfo;

typedef struct {
	uint8_t level;
	uint8_t speed;
	uint8_t undead;
	uint8_t body_attack;
	uint16_t pad;
	uint16_t mad;
	uint16_t pdd;
	uint16_t mdd;
	uint32_t exp;
	uint32_t hp;
	uint32_t max_hp;
	uint32_t mp;
	uint32_t max_mp;
	std::string name;
} MonsterInfo;