#include "../../pch.h"
#include "item_info.h"

ItemInfo::ItemInfo()
{
}

ItemInfo::~ItemInfo()
{
}

int32_t ItemInfo::GetReqLevel() const
{
	return _req_level;
}

int32_t ItemInfo::GetReqStr() const
{
	return _req_str;
}

int32_t ItemInfo::GetReqDex() const
{
	return _req_dex;
}

int32_t ItemInfo::GetReqInt() const
{
	return _req_int;
}

int32_t ItemInfo::GetReqLuk() const
{
	return _req_luk;
}

int32_t ItemInfo::GetIncStr() const
{
	return _inc_str;
}

int32_t ItemInfo::GetIncDex() const
{
	return _inc_dex;
}

int32_t ItemInfo::GetIncInt() const
{
	return _inc_int;
}

int32_t ItemInfo::GetIncLuk() const
{
	return _inc_luk;
}

int32_t ItemInfo::GetIncPad() const
{
	return _inc_pad;
}

int32_t ItemInfo::GetIncMad() const
{
	return _inc_mad;
}

int32_t ItemInfo::GetIncPdd() const
{
	return _inc_pdd;
}

int32_t ItemInfo::GetIncMdd() const
{
	return _inc_mdd;
}

void ItemInfo::SetReqLevel(int32_t level)
{
	_req_level = level;
}

void ItemInfo::SetReqStr(int32_t str)
{
	_req_str = str;
}

void ItemInfo::SetReqDex(int32_t dex)
{
	_req_dex = dex;
}

void ItemInfo::SetReqInt(int32_t int_)
{
	_req_int = int_;
}

void ItemInfo::SetReqLuk(int32_t luk)
{
	_req_luk = luk;
}

void ItemInfo::SetIncStr(int32_t str)
{
	_inc_str = str;
}

void ItemInfo::SetIncDex(int32_t dex)
{
	_inc_dex = dex;
}

void ItemInfo::SetIncInt(int32_t int_)
{
	_inc_int = int_;
}

void ItemInfo::SetIncLuk(int32_t luk)
{
	_inc_luk = luk;
}

void ItemInfo::SetIncPad(int32_t pad)
{
	_inc_pad = pad;
}

void ItemInfo::SetIncMad(int32_t mad)
{
	_inc_mad = mad;
}

void ItemInfo::SetIncPdd(int32_t pdd)
{
	_inc_pdd = pdd;
}

void ItemInfo::SetIncMdd(int32_t mdd)
{
	_inc_mdd = mdd;
}
