#include "../../pch.h"
#include "item_info.h"

ItemInfo::ItemInfo(): _tuc(0),
                      _req_level(0),
                      _req_str(0),
                      _req_dex(0),
                      _req_int(0),
                      _req_luk(0),
                      _inc_str(0),
                      _inc_dex(0),
                      _inc_int(0),
                      _inc_luk(0),
                      _inc_pad(0),
                      _inc_mad(0),
                      _inc_pdd(0),
                      _inc_mdd(0),
                      _inc_mhp(0),
                      _inc_mmp(0),
                      _price(0)
{
}

ItemInfo::~ItemInfo()
{
}

int8_t ItemInfo::GetTuc() const
{
	return _tuc;
}

int8_t ItemInfo::GetSucessTuc() const
{
	return _success_tuc;
}

void ItemInfo::SetSucessTuc(int8_t tuc)
{
	_success_tuc = tuc;
}

int16_t ItemInfo::GetReqLevel() const
{
	return _req_level;
}

int16_t ItemInfo::GetReqStr() const
{
	return _req_str;
}

int16_t ItemInfo::GetReqDex() const
{
	return _req_dex;
}

int16_t ItemInfo::GetReqInt() const
{
	return _req_int;
}

int16_t ItemInfo::GetReqLuk() const
{
	return _req_luk;
}

int16_t ItemInfo::GetIncStr() const
{
	return _inc_str;
}

int16_t ItemInfo::GetIncDex() const
{
	return _inc_dex;
}

int16_t ItemInfo::GetIncInt() const
{
	return _inc_int;
}

int16_t ItemInfo::GetIncLuk() const
{
	return _inc_luk;
}

int16_t ItemInfo::GetIncPad() const
{
	return _inc_pad;
}

int16_t ItemInfo::GetIncMad() const
{
	return _inc_mad;
}

int16_t ItemInfo::GetIncPdd() const
{
	return _inc_pdd;
}

int16_t ItemInfo::GetIncMdd() const
{
	return _inc_mdd;
}

int16_t ItemInfo::GetIncMhp() const
{
	return _inc_mhp;
}

int16_t ItemInfo::GetIncMmp() const
{
	return _inc_mmp;
}

int32_t ItemInfo::GetPrice() const
{
	return _price;
}

void ItemInfo::SetTuc(int8_t tuc)
{
	_tuc = tuc;
}

void ItemInfo::SetReqLevel(int16_t level)
{
	_req_level = level;
}

void ItemInfo::SetReqStr(int16_t str)
{
	_req_str = str;
}

void ItemInfo::SetReqDex(int16_t dex)
{
	_req_dex = dex;
}

void ItemInfo::SetReqInt(int16_t int_)
{
	_req_int = int_;
}

void ItemInfo::SetReqLuk(int16_t luk)
{
	_req_luk = luk;
}

void ItemInfo::SetIncStr(int16_t str)
{
	_inc_str = str;
}

void ItemInfo::SetIncDex(int16_t dex)
{
	_inc_dex = dex;
}

void ItemInfo::SetIncInt(int16_t int_)
{
	_inc_int = int_;
}

void ItemInfo::SetIncLuk(int16_t luk)
{
	_inc_luk = luk;
}

void ItemInfo::SetIncPad(int16_t pad)
{
	_inc_pad = pad;
}

void ItemInfo::SetIncMad(int16_t mad)
{
	_inc_mad = mad;
}

void ItemInfo::SetIncPdd(int16_t pdd)
{
	_inc_pdd = pdd;
}

void ItemInfo::SetIncMdd(int16_t mdd)
{
	_inc_mdd = mdd;
}

void ItemInfo::SetIncMhp(int16_t mhp)
{
	_inc_mhp = mhp;
}

void ItemInfo::SetIncMmp(int16_t mmp)
{
	_inc_mmp = mmp;
}

void ItemInfo::SetPrice(int32_t price)
{
	_price = price;
}
