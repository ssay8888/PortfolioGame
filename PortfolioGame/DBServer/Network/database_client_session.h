#pragma once

class Character;
class OutPacket;

class DatabaseClientSession final : public std::enable_shared_from_this<DatabaseClientSession>
{
public:
	enum class ServerStatus { kConnect = 0x01, kClose = 0x02 };
	DatabaseClientSession(boost::asio::io_service& server_service);
	virtual ~DatabaseClientSession();

	virtual void SessionHandleRecv(const std::error_code& error_code, const size_t bytes_transferred, uint8_t* buffer);
	void ProcessPacket(const std::error_code& error_code, std::size_t bytes_transferred, uint8_t* buffer, int nPacketLen);
	void OnSendPacketFinished(const std::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<OutPacket> out_packet);
	void WaitRecvPacket() noexcept;
	void OnDisconnect();
	void Init();
	void SendPacket(std::shared_ptr<OutPacket> out_packet, bool is_broad_cast = false);
public:
	boost::asio::ip::tcp::socket& GetSocket();
	uint64_t GetSocketId() const;
	void SetSocketDisconnectedCallBack(const std::function<void(std::shared_ptr<DatabaseClientSession>)>& callback);
	void SetCharacter(std::shared_ptr<Character> character);
	std::shared_ptr<Character> GetCharacter() const;
	std::shared_ptr<DatabaseClientSession> GetThis();
	void SetThis(std::shared_ptr<DatabaseClientSession> ptr);
private:
	std::mutex _mtx_lock;
	uint64_t _socket_id;
	boost::asio::ip::tcp::socket _socket;
	std::function<void(std::shared_ptr<DatabaseClientSession>)> _socket_disconnected_callback;
	std::shared_ptr<Character> _character;
	std::shared_ptr<DatabaseClientSession> _this;
	ServerStatus _status;
};
