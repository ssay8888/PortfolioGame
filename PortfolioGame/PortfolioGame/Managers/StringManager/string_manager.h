#pragma once
class StringInfo;
class StringManager
{
	StringManager();
	~StringManager();
public:
	static StringManager* GetInstance()
	{
		static StringManager instance;
		return &instance;
	}

	void InsertStringInfo(StringInfo info);
	void LoadStringInfo();
	StringInfo FindItemStringInfo(int32_t item_id);

private:
	std::map<int32_t, StringInfo> _list;
};

