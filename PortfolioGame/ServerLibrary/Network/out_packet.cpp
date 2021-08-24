#include "../pch.h"
#include "out_packet.h"
#include <Windows.h>
#include <minwinbase.h>
#include <winnt.h>


OutPacket::OutPacket(): _buffer(new uint8_t[kDefaultBuffSize]),
                        _packet_size(kInitIndex)
{
	ZeroMemory(_buffer, kDefaultBuffSize);
}

OutPacket::~OutPacket()
{
	delete _buffer;
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
	*(decltype(value)*)(_buffer + _packet_size) = value;
	_packet_size += sizeof(value);
}

void OutPacket::Encode2(short value)
{

	*(decltype(value)*)(_buffer + _packet_size) = value;
	_packet_size += sizeof(value);
}
void OutPacket::Encode4(int value)
{

	*(decltype(value)*)(_buffer + _packet_size) = value;
	_packet_size += sizeof(value);
}
void OutPacket::Encode8(long long value)
{

	*(decltype(value)*)(_buffer + _packet_size) = value;
	_packet_size += sizeof(value);
}

void OutPacket::EncodeStr(const std::string& str)
{
	Encode2((short)str.size());

	memcpy(_buffer + _packet_size, str.c_str(), str.size());
	_packet_size += (unsigned int)str.size();
}
