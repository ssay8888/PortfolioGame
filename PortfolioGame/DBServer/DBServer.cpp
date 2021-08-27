#include "pch.h"
#include <iostream>

#include "Network/database_server.h"
#include "Network/PacketHandlerManager/database_packet_handler_manager.h"

void ConnectionAcceptorThread()
{
	auto* loginSocket = DatabaseServer::GetInstance();
	loginSocket->CreateAcceptor();
	loginSocket->AsyncStartAcceptor();
}

int main()
{
	srand(time(NULL));
	DatabasePacketHandlerManager::GetInstance()->LoadHandlers();
	auto* loginSocket = DatabaseServer::GetInstance();
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