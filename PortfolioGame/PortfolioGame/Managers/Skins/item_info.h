#pragma once
class ItemInfo
{
public:
	ItemInfo();
	~ItemInfo();

	int32_t GetReqLevel() const;
	int32_t GetReqStr() const;
	int32_t GetReqDex() const;
	int32_t GetReqInt() const;
	int32_t GetReqLuk() const;
	int32_t GetIncStr() const;
	int32_t GetIncDex() const;
	int32_t GetIncInt() const;
	int32_t GetIncLuk() const;
	int32_t GetIncPad() const;
	int32_t GetIncMad() const;
	int32_t GetIncPdd() const;
	int32_t GetIncMdd() const;

	void SetReqLevel(int32_t level);
	void SetReqStr(int32_t str);
	void SetReqDex(int32_t dex);
	void SetReqInt(int32_t int_);
	void SetReqLuk(int32_t luk);
	void SetIncStr(int32_t str);
	void SetIncDex(int32_t dex);
	void SetIncInt(int32_t int_);
	void SetIncLuk(int32_t luk);
	void SetIncPad(int32_t pad);
	void SetIncMad(int32_t mad);
	void SetIncPdd(int32_t pdd);
	void SetIncMdd(int32_t mdd);
	
private:
	int32_t _req_level;
	int32_t _req_str;
	int32_t _req_dex;
	int32_t _req_int;
	int32_t _req_luk;
	int32_t _inc_str;
	int32_t _inc_dex;
	int32_t _inc_int;
	int32_t _inc_luk;
	int32_t _inc_pad;
	int32_t _inc_mad;
	int32_t _inc_pdd;
	int32_t _inc_mdd;

};

