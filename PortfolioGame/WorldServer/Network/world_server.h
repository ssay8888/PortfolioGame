#pragma once
class WorldClientSession;

class WorldServer 
{
	WorldServer();
	~WorldServer();
public:

	void CreateAcceptor();
	
	static WorldServer* GetInstance()
	{
		static WorldServer instance;
		return &instance;
	}

	boost::asio::ip::tcp::acceptor* GetAcceptor()
	{
		return _acceptor;
	}
	
	void HandleAcceptor(std::shared_ptr<WorldClientSession> connected_socket,
		const std::error_code& error_code);

  void AsyncStartAcceptor();
  void OnSocketDisconnected(std::shared_ptr<WorldClientSession> socket);
	virtual void OnNotifySocketDisconnected(std::shared_ptr<WorldClientSession> socket);
	void RunWorkThead();
	void InsertWorkPacket(std::shared_ptr<WorldClientSession> session, std::shared_ptr<InPacket> in_packet);
	void BroadCastMessage(std::shared_ptr<OutPacket> out_packet, std::shared_ptr<WorldClientSession> session);
	static std::map<int64_t, std::shared_ptr<WorldClientSession>>& GetSessionList();
public:
	boost::asio::io_service& GetIoService();


private:
	boost::asio::io_service _io_service;
	boost::asio::ip::tcp::acceptor* _acceptor;
	static std::map<int64_t, std::shared_ptr<WorldClientSession>> _session_list;
	std::queue<std::pair<std::shared_ptr<WorldClientSession>, std::shared_ptr<InPacket>>> _work_packet_list;
	std::mutex _mtx_lock;
};
