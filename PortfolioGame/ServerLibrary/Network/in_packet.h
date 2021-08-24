#pragma once
class InPacket
{
public:
	InPacket(uint8_t* buff, uint16_t size);
	~InPacket();
	char Decode1();
	short Decode2();
	int Decode4();
	long long int Decode8();
	std::string DecodeStr();
	void DecodeBuffer(uint8_t* dst, int32_t size);

	unsigned char* GetPacket() const;

	unsigned short GetPacketSize() const;
	
	unsigned short GetReadCount() const;

	void RestorePacket();

	void Seek(int nPos);

	void Offset(int nOffset);

	void Print();
private:
	uint8_t* _buffer;
	uint16_t _packet_size, _read_pos;

};

