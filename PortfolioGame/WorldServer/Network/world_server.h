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
	
	void HandleAcceptor(WorldClientSession* connected_socket,
		const std::error_code& error_code);

  void AsyncStartAcceptor();
  void OnSocketDisconnected(WorldClientSession* socket);
	virtual void OnNotifySocketDisconnected(WorldClientSession* socket);
	void RunWorkThead();
	void InsertWorkPacket(WorldClientSession* session, InPacket* in_packet);

public:
	boost::asio::io_service& GetIoService();


private:
	boost::asio::io_service _io_service;
	boost::asio::ip::tcp::acceptor* _acceptor;
	static std::map<int64_t, WorldClientSession*> _session_list;
	std::queue<std::pair<WorldClientSession*, InPacket*>> _work_packet_list;
};
