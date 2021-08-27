#pragma once
class DbServer
{
public:
	static void ConnectionAcceptorThread();
	void DbInit();
	void Init();
	void SelectTest();
};

