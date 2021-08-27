#pragma once

class DatabaseClientSession;

class DatabaseServer
{
	DatabaseServer();
	~DatabaseServer();
public:

	void CreateAcceptor();

	static DatabaseServer* GetInstance()
	{
		static DatabaseServer instance;
		return &instance;
	}

	boost::asio::ip::tcp::acceptor* GetAcceptor()
	{
		return _acceptor;
	}

	void HandleAcceptor(std::shared_ptr<DatabaseClientSession> connected_socket,
		const std::error_code& error_code);

	void AsyncStartAcceptor();
	void OnSocketDisconnected(std::shared_ptr<DatabaseClientSession> socket);
	virtual void OnNotifySocketDisconnected(std::shared_ptr<DatabaseClientSession> socket);
	void RunWorkThead();
	void InsertWorkPacket(std::shared_ptr<DatabaseClientSession> session, std::shared_ptr<InPacket> in_packet);
	void BroadCastMessage(std::shared_ptr<OutPacket> out_packet, std::shared_ptr<DatabaseClientSession> session);
	static std::map<int64_t, std::shared_ptr<DatabaseClientSession>>& GetSessionList();

public:
	boost::asio::io_service& GetIoService();


private:
	boost::asio::io_service _io_service;
	boost::asio::ip::tcp::acceptor* _acceptor;
	static std::map<int64_t, std::shared_ptr<DatabaseClientSession>> _session_list;
	std::queue<std::pair<std::shared_ptr<DatabaseClientSession>, std::shared_ptr<InPacket>>> _work_packet_list;
	std::mutex _mtx_lock;
};

