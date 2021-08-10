#pragma once
class AniMapObject;
class Portal
{
public:
	Portal(ObjectPos pos, int32_t next, Info info);
	~Portal();

	void RenderPortal(HDC hdc);

	void SetNextMap(const int32_t next);
	int32_t GetNextMap() const;

	void SetNextMapSpawnPos(const ObjectPos pos);
	ObjectPos GetNextMapSpawnPos() const;

	Info GetInfo() const;
	void SetInfo(Info info);
	float GetInfoX() const;
	float GetInfoY() const;
	int32_t GetInfoCX() const;
	int32_t GetInfoCY() const;

	void SetInfoX(float x);
	void SetInfoY(float y);
	void SetInfoCX(int cx);
	void SetInfoCY(int cy);

	RECT GetRect() const;

private:
	int32_t _next_map;
	ObjectPos _next_map_spawn_pos;
	AniMapObject* _ani_image;
	Info _info;
	RECT _rect;
};

