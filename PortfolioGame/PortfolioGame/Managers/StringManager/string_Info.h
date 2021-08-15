#pragma once
class StringInfo
{
public:
	StringInfo();
	~StringInfo() = default;
	 
	void SetItemId(int32_t item_id);
	void SetName(std::wstring name);
	void SetDesc(std::wstring desc);

	int32_t GetItemId() const;
	std::wstring GetName() const;
	std::wstring GetDesc() const;
private:
	int32_t _item_id;
	std::wstring _name;
	std::wstring _desc;
};

