#pragma once
#include <sqltypes.h>
class DatabaseConnector;
class PreparedStatement;
class Connection
{
public:
	Connection(DatabaseConnector& connector);
	~Connection();
	PreparedStatement& GetPreparedStatement();
	SQLHDBC& GetConnectionHandle();
private:
	void ThrowException();
private:
	SQLHDBC _connection_handle{};
};

