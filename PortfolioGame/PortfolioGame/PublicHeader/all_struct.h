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
} ObjectInfo;