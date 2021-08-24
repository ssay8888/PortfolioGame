#pragma once

class OutPacket;

class WorldClientSession final : public std::enable_shared_from_this<WorldClientSession>
{

public:
	WorldClientSession(boost::asio::io_service& server_service);
	virtual ~WorldClientSession();

	virtual void SessionHandleRecv(const std::error_code& error_code, const size_t bytes_transferred, uint8_t* buffer);
	void ProcessPacket(const std::error_code& error_code, std::size_t bytes_transferred, uint8_t* buffer, int nPacketLen);
	void OnSendPacketFinished(const std::error_code& ec, std::size_t bytes_transferred, OutPacket* out_packet);
	void WaitRecvPacket() noexcept;
	void OnDisconnect();
	void Init();
	void SendPacket(OutPacket* out_packet);
public:
	boost::asio::ip::tcp::socket& GetSocket();
	uint64_t GetSocketId() const;
	void SetSocketDisconnectedCallBack(const std::function<void(WorldClientSession*)>& callback);

private:
	std::mutex _mtx_lock;
	uint64_t _socket_id;
	boost::asio::ip::tcp::socket _socket;
	std::function<void(WorldClientSession*)> _socket_disconnected_callback;
};
