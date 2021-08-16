#pragma once
class ItemInfo
{
public:
	ItemInfo();
	~ItemInfo();

	int16_t GetReqLevel() const;
	int16_t GetReqStr() const;
	int16_t GetReqDex() const;
	int16_t GetReqInt() const;
	int16_t GetReqLuk() const;
	int16_t GetIncStr() const;
	int16_t GetIncDex() const;
	int16_t GetIncInt() const;
	int16_t GetIncLuk() const;
	int16_t GetIncPad() const;
	int16_t GetIncMad() const;
	int16_t GetIncPdd() const;
	int16_t GetIncMdd() const;
	int16_t GetIncMhp() const;
	int16_t GetIncMmp() const;

	void SetReqLevel(int16_t level);
	void SetReqStr(int16_t str);
	void SetReqDex(int16_t dex);
	void SetReqInt(int16_t int_);
	void SetReqLuk(int16_t luk);
	void SetIncStr(int16_t str);
	void SetIncDex(int16_t dex);
	void SetIncInt(int16_t int_);
	void SetIncLuk(int16_t luk);
	void SetIncPad(int16_t pad);
	void SetIncMad(int16_t mad);
	void SetIncPdd(int16_t pdd);
	void SetIncMdd(int16_t mdd);
	void SetIncMhp(int16_t mhp);
	void SetIncMmp(int16_t mmp);
	
private:
	int16_t _req_level;
	int16_t _req_str;
	int16_t _req_dex;
	int16_t _req_int;
	int16_t _req_luk;
	int16_t _inc_str;
	int16_t _inc_dex;
	int16_t _inc_int;
	int16_t _inc_luk;
	int16_t _inc_pad;
	int16_t _inc_mad;
	int16_t _inc_pdd;
	int16_t _inc_mdd;
	int16_t _inc_mhp;
	int16_t _inc_mmp;

};

