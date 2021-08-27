#include "pch.h"
#include "db_server.h"
#include "Database/database_connector.h"
#include "Database/connection.h"
#include "Database/prepared_statement.h"
#include "Database/result_set.h"
#include "Network/database_server.h"
#include "Network/PacketHandlerManager/database_packet_handler_manager.h"

void DbServer::ConnectionAcceptorThread() {
	auto* loginSocket = DatabaseServer::GetInstance();
	loginSocket->CreateAcceptor();
	loginSocket->AsyncStartAcceptor();
}

void DbServer::DbInit()
{
	std::string db_name("PortfolioDatabase");
	std::string db_id("root");
	std::string db_pw("1597748x");
  
	DatabaseConnector::GetInstance()->SetData(db_name.c_str(), db_id.c_str(), db_pw.c_str());
	std::cout << "Database Connector Initialize..." << std::endl;
	DatabaseConnector::GetInstance()->Init();
	std::cout << "Database Connector Initialize DONE !!!" << std::endl;;
}

void DbServer::Init()
{
	srand(time(NULL));
	DbInit();
	SelectTest();
	DatabasePacketHandlerManager::GetInstance()->LoadHandlers();
	auto* database_server = DatabaseServer::GetInstance();
	database_server->RunWorkThead();
	boost::asio::io_service& io = database_server->GetIoService();
	boost::asio::io_service::work work(io);
	std::thread init_login_server_thread(ConnectionAcceptorThread);

	while (true)
	{
		boost::system::error_code error_code;
		io.run(error_code);
	}
}

void DbServer::SelectTest() {
	try {
		const auto database_connector = DatabaseConnector::GetInstance();
		Connection& connection = database_connector->GetConnection();
		PreparedStatement& ps = connection.GetPreparedStatement();

		ps.PrepareStatement("select account, password from accounts where account = ?");
		std::string id("test2");
		ps.SetString(1, id);
		const auto rs = ps.Execute();

		char account[64];
		char password[64];
		rs->GetString(1, account, 64);
		rs->GetString(2, password, 64);
		if (rs->Fetch()) {
			std::cout << account << std::endl;
			std::cout << password << std::endl;
		}
		rs->Close();
		ps.Close();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
