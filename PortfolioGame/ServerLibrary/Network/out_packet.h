#pragma once
class OutPacket
{
public:
	constexpr static int kPacketHeaderSize = 4;
	constexpr static int kDefaultBuffSize = 256;
	constexpr static int kInitIndex = kPacketHeaderSize;

	OutPacket();
	~OutPacket();

	unsigned char* GetPacket() const;
	uint32_t GetPacketSize() const; 
	void Encode1(char value);
	void Encode2(short value);
	void Encode4(int value);
	void Encode8(long long value);
	void EncodeStr(const std::string& str);
	void ExtendSize();
private:
	uint8_t* _buffer;
	uint32_t _packet_size;
	uint32_t _buffer_size;
};

