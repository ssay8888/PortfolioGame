#pragma once

class Character;
class OutPacket;

class WorldClientSession final : public std::enable_shared_from_this<WorldClientSession>
{
public:
  enum class ServerStatus { kConnect = 0x01, kClose= 0x02};
	WorldClientSession(boost::asio::io_service& server_service);
	virtual ~WorldClientSession();

  void HandleConnect(const boost::system::error_code& error_code);
  void PacketHeaderReceive() noexcept;
  void PacketBodyReceive(const std::error_code& error_code, size_t bytes_transferred,
                         uint8_t* buffer) noexcept;
	virtual void SessionHandleRecv(const std::error_code& error_code, const size_t bytes_transferred, uint8_t* buffer);
	void ProcessPacket(const std::error_code& error_code, std::size_t bytes_transferred, uint8_t* buffer, int nPacketLen);
	void OnSendPacketFinished(const std::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<OutPacket> out_packet);
	void WaitRecvPacket() noexcept;
	void OnDisconnect();
	void Init();
	void SendPacket(std::shared_ptr<OutPacket> out_packet, bool is_broad_cast = false);
public:
	boost::asio::ip::tcp::socket& GetSocket();
  void Connect(boost::asio::ip::tcp::endpoint endpoint);
  uint64_t GetSocketId() const;
	void SetSocketDisconnectedCallBack(const std::function<void(std::shared_ptr<WorldClientSession>)>& callback);
	void SetCharacter(std::shared_ptr<Character> character);
	std::shared_ptr<Character> GetCharacter() const;
	std::shared_ptr<WorldClientSession> GetThis();
	void SetThis(std::shared_ptr<WorldClientSession> ptr);
private:
	std::mutex _mtx_lock;
	uint64_t _socket_id;
	boost::asio::ip::tcp::socket _socket;
	std::function<void(std::shared_ptr<WorldClientSession>)> _socket_disconnected_callback;
	std::shared_ptr<Character> _character;
	std::shared_ptr<WorldClientSession> _this;
	ServerStatus _status;
};
