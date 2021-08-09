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