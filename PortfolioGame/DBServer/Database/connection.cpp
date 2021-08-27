#include "../pch.h"
#include "connection.h"
#include "database_connector.h"
#include "prepared_statement.h"
#include <sql.h>

Connection::Connection(DatabaseConnector& connector) {
	if (SQLAllocHandle(SQL_HANDLE_DBC, connector.GetEnvironmentHandle(), &_connection_handle) != SQL_SUCCESS) {
		throw std::logic_error("Exception) SQL Connection handle allocation Fail...");
	}

	SQLRETURN Ret = SQLConnect(
		_connection_handle,
		connector.GetDataSourceName(), SQL_NTS,
		connector.GetId(), SQL_NTS,
		connector.GetPw(), SQL_NTS);
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO)) {
		ThrowException();
	}
}

Connection::~Connection() {
	if (_connection_handle) {
		SQLDisconnect(_connection_handle);
		SQLFreeHandle(SQL_HANDLE_DBC, _connection_handle);
	}
}

PreparedStatement& Connection::GetPreparedStatement()
{
	static thread_local PreparedStatement ps(*this);
	return ps;
}

SQLHDBC& Connection::GetConnectionHandle()
{
	return _connection_handle;
}

void Connection::ThrowException()
{
	SQLINTEGER native_error;
	SQLSMALLINT msg_len;
	char mes[1024] {0, };

	for (int diag = 1; ; diag++) {
		SQLCHAR msg[255];
		SQLCHAR sql_state[6];
		SQLRETURN Ret = SQLGetDiagRec(SQL_HANDLE_DBC, _connection_handle, diag, sql_state,
		                              &native_error, msg, sizeof(msg), &msg_len);
		if (Ret == SQL_NO_DATA)
			break;
		sprintf_s(mes, 1024, "SQLSTATE:%s, NativeError:%d, 진단정보:%s", sql_state, native_error, msg);
	}
	std::string str("Exception) SQL Connection Fail...\n");
	str.append(mes);
	throw std::logic_error(str.c_str());
}
