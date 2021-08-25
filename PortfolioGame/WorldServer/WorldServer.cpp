#include "pch.h"
#include <iostream>
#include <thread>

#include "Network/world_server.h"
#include "Network/PacketHandlerManager/world_packet_handler_manager.h"

void ConnectionAcceptorThread()
{
	auto* loginSocket = WorldServer::GetInstance();
	loginSocket->CreateAcceptor();
	loginSocket->AsyncStartAcceptor();
}

int main()
{
	srand(time(NULL));
	WorldPacketHandlerManager::GetInstance()->LoadHandlers();
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
