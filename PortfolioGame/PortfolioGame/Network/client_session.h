#pragma once
#include <queue>
class Player;
class OutPacket;

class ClientSession
{
public:
	ClientSession(boost::asio::io_service& io_service);
	~ClientSession();
	void Connect(boost::asio::ip::tcp::endpoint endpoint);
	void HandleConnect(const boost::system::error_code& error_code);
	void PacketHeaderReceive() noexcept;
	void PacketBodyReceive(const std::error_code& error_code, const size_t bytes_transferred, uint8_t* buffer) noexcept;
	void ProcessPacket(const std::error_code& error_code, const size_t bytes_transferred, uint8_t* buffer, int32_t packet_len) noexcept;
	void OnSendPacketFinished(const std::error_code& ec, std::size_t bytes_transferred, OutPacket* out_packet);
	void SendPacket(OutPacket* out_packet);

	void RunWorkThead();

public:
	void SetPlayer(Player* player);
	Player* GetPlayer() const;
private:
	Player* _player;
	boost::asio::io_service& _io_service;
	boost::asio::ip::tcp::socket _socket;
	std::mutex _mtx_lock;

	std::queue<InPacket*> _work_packet_list;

};

