#pragma once
class Character
{
public:
	Character();
	~Character();
	void SetId(int32_t id);
	int32_t GetId() const;
	void SetPos(POINT pos);
	POINT GetPos() const;
private:
	int32_t _id;
	POINT _pos;

};

