#include "../pch.h"
#include "out_packet.h"
#include <Windows.h>
#include <minwinbase.h>
#include <winnt.h>


OutPacket::OutPacket(): _buffer(new uint8_t[kDefaultBuffSize]),
                        _packet_size(kInitIndex),
												_buffer_size(kDefaultBuffSize)
{
	ZeroMemory(_buffer, kDefaultBuffSize);
}

OutPacket::~OutPacket()
{
	delete[] _buffer;
}
unsigned char* OutPacket::GetPacket() const {
  return _buffer + kInitIndex;
}

uint32_t OutPacket::GetPacketSize() const
{
	return _packet_size;
}

void OutPacket::Encode1(char value)
{

	if (_packet_size + sizeof(value) >= _buffer_size)
		ExtendSize();

	*(decltype(value)*)(_buffer + _packet_size) = value;
	_packet_size += sizeof(value);
}

void OutPacket::Encode2(short value)
{
	if (_packet_size + sizeof(value) >= _buffer_size)
		ExtendSize();
	*(decltype(value)*)(_buffer + _packet_size) = value;
	_packet_size += sizeof(value);
}
void OutPacket::Encode4(int value)
{
	if (_packet_size + sizeof(value) >= _buffer_size)
		ExtendSize();
	*(decltype(value)*)(_buffer + _packet_size) = value;
	_packet_size += sizeof(value);
}
void OutPacket::Encode8(long long value)
{
	if (_packet_size + sizeof(value) >= _buffer_size)
		ExtendSize();
	*(decltype(value)*)(_buffer + _packet_size) = value;
	_packet_size += sizeof(value);
}

void OutPacket::EncodeStr(const std::string& str)
{
	Encode2(static_cast<short>(str.size()));

	if (_packet_size + str.size() >= _buffer_size)
		ExtendSize();
	memcpy(_buffer + _packet_size, str.c_str(), str.size());
	_packet_size += static_cast<unsigned>(str.size());
}

void OutPacket::ExtendSize()
{
	const auto newBuff = new uint8_t[_buffer_size * 2];
	memcpy(newBuff, _buffer, _packet_size);
	delete[] _buffer;
	_buffer_size *= 2;
	_buffer = newBuff;
}
