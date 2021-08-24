#include "../pch.h"
#include "in_packet.h"
InPacket::InPacket(uint8_t* buff, uint16_t size): _buffer(buff),
                                                  _packet_size(size),
                                                  _read_pos(0)
{
}

InPacket::~InPacket()
{
	delete _buffer;
}

char InPacket::Decode1()
{
	if (_read_pos >= _packet_size || _read_pos + sizeof(char) > _packet_size)
		throw std::runtime_error("Access Error InPacket::Decode1");

	char ret = *reinterpret_cast<char*>(_buffer + _read_pos);
	_read_pos += sizeof(char);
	return ret;
}
short InPacket::Decode2()
{
	if (_read_pos >= _packet_size || _read_pos + sizeof(short) > _packet_size)
		throw std::runtime_error("Access Error InPacket::Decode2");

	short ret = *reinterpret_cast<short*>(_buffer + _read_pos);
	_read_pos += sizeof(short);
	return ret;
}
int InPacket::Decode4()
{
	if (_read_pos >= _packet_size || _read_pos + sizeof(int) > _packet_size)
		throw std::runtime_error("Access Error InPacket::Decode2");

	int ret = *reinterpret_cast<int*>(_buffer + _read_pos);
	_read_pos += sizeof(int);
	return ret;
}
long long InPacket::Decode8()
{
	if (_read_pos >= _packet_size || _read_pos + sizeof(long long) > _packet_size)
		throw std::runtime_error("Access Error InPacket::Decode2");

	long long ret = *reinterpret_cast<long long*>(_buffer + _read_pos);
	_read_pos += sizeof(long long);
	return ret;
}
std::string InPacket::DecodeStr() {
	if (_read_pos >= _packet_size || _read_pos + sizeof(short) > _packet_size)
		throw std::runtime_error("Access Error InPacket::DecodeStr (Size Header)");

	short size = Decode2();
	if (_read_pos >= _packet_size || _read_pos + size > _packet_size)
		throw std::runtime_error("Access Error InPacket::DecodeStr (Str Data)");

	std::string ret(reinterpret_cast<char*>(_buffer) + _read_pos, size);
	_read_pos += size;
	return ret;
}
void InPacket::DecodeBuffer(uint8_t* dst, int32_t size)
{
	if (_read_pos >= _packet_size || _read_pos + size > _packet_size)
		throw std::runtime_error("Access Error InPacket::DecodeBuffer");

	if (dst)
		memcpy(dst, _buffer + _read_pos, size);
	_read_pos += size;
}
unsigned char* InPacket::GetPacket() const
{
	return _buffer;
}
unsigned short InPacket::GetPacketSize() const
{
	return _packet_size;
}
unsigned short InPacket::GetReadCount() const
{
	return _read_pos;
}
void InPacket::RestorePacket()
{
	_read_pos = 0;
}
void InPacket::Seek(int nPos)
{
	if (nPos >= 0 && nPos < _packet_size)
		_read_pos = nPos;
}
void InPacket::Offset(int nOffset) {
	_read_pos += nOffset;
	if (_read_pos < 0 || _read_pos >= _packet_size)
		_read_pos -= nOffset;
}
void InPacket::Print() {}
