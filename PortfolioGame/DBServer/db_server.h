#pragma once
#include "Database/database_connector.h"

#include "Network/database_server.h"

class DbServer
{
public:
	static void ConnectionAcceptorThread();
	void DbInit();
	void Init();
	void SelectTest();
};

