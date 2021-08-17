#pragma once
#include "../../Base/game_object.h"
class Npc final :
    public GameObject
{
public:
	Npc();
	~Npc();

	void InsertImage(std::shared_ptr<MyBitmap> image);
	std::vector<std::shared_ptr<MyBitmap>>& GetStandImages();

	void SetOrigin(POINT point);
	void SetOriginX(int32_t x);
	void SetOriginY(int32_t y);
	void SetNpcId(int32_t npc_id);
	void SetNpcPath(std::string path);

	POINT GetOrigin() const;
	int32_t GetNpcId() const;
	std::string GetNpcPath() const;
private:
	int ReadyGameObject() override;
	void UpdateGameObject(const float deltaTime) override;
	void RenderGameObject(HDC hdc) override;
	void LateUpdateGameObject() override;

	void QuestCheckRender(HDC hdc);
private:
	int32_t _npc_id;
	std::string _npc_path;
	std::vector<std::shared_ptr<MyBitmap>> _stand;
	POINT _origin;
	uint64_t _frame_tick;
	uint32_t _frame_number;
};

