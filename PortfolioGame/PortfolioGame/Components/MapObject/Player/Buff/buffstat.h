#pragma once

class BuffStat
{
public:
	BuffStat();
	~BuffStat();
	void AddFlag(ObjectType::BuffFlag flag);
	void RemoveFlag(ObjectType::BuffFlag flag);
	bool CheckFlag(ObjectType::BuffFlag flag) const;
	
private:
	int32_t _flag;
};

