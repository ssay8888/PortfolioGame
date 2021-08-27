#include "pch.h"

#include "Network/world_server.h"
#include "Network/PacketHandlerManager/world_packet_handler_manager.h"
#include "Network/world_client_session.h"
std::shared_ptr<WorldClientSession> client_session = nullptr;
constexpr int kDbServerPort = 1001;

void ConnectionAcceptorThread()
{
	auto* loginSocket = WorldServer::GetInstance();
	loginSocket->CreateAcceptor();
	loginSocket->AsyncStartAcceptor();
}

void DbServerConnect()
{
	boost::asio::io_service io_service;
	auto endpoint = boost::asio::ip::tcp::endpoint(
		boost::asio::ip::address::from_string("127.0.0.1"),
		kDbServerPort);
	client_session = std::make_shared<WorldClientSession>(io_service);
	client_session->SetThis(client_session);
	client_session->Connect(endpoint);
	boost::asio::io_service::work work(io_service);
	std::thread clientWorkThread([&] {
		boost::system::error_code ec;
		io_service.run(ec);
		});
	clientWorkThread.detach();
}

int main()
{
	srand(time(NULL));
	WorldPacketHandlerManager::GetInstance()->LoadHandlers();

	DbServerConnect();


	auto* loginSocket = WorldServer::GetInstance();
	loginSocket->RunWorkThead();
	std::thread init_login_server_thread(ConnectionAcceptorThread);

	boost::asio::io_service& io = loginSocket->GetIoService();
	boost::asio::io_service::work work(io);


	while (true)
	{
		boost::system::error_code error_code;
		io.run(error_code);
	}

}
