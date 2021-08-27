#include "../pch.h"
#include "prepared_statement.h"
#include "connection.h"
#include "result_set.h"

PreparedStatement::PreparedStatement(Connection& con)
	: _nts_length(SQL_NTS)
{
	if (SQLAllocHandle(SQL_HANDLE_STMT,
		con.GetConnectionHandle(), &_statement_handle) != SQL_SUCCESS) {
		throw std::logic_error("Exception) SQL Command handle allocation Fail...");
	}
}

PreparedStatement::~PreparedStatement() {
	if (_statement_handle)
		SQLFreeHandle(SQL_HANDLE_STMT, _statement_handle);
}

void PreparedStatement::PrepareStatement(const char* string) {
	_ret = SQLPrepare(_statement_handle, (SQLCHAR*)string, SQL_NTS);
	if ((_ret != SQL_SUCCESS) && (_ret != SQL_SUCCESS_WITH_INFO)) {
		ThrowException();
	}
}

void PreparedStatement::SetInt(int index, int value) {
	_ret = SQLBindParameter(
		_statement_handle,
		index,
		SQL_PARAM_INPUT,
		SQL_C_SLONG,
		SQL_INTEGER, 0, 0, &value, 0, NULL);
	if ((_ret != SQL_SUCCESS) && (_ret != SQL_SUCCESS_WITH_INFO)) {
		ThrowException();
	}
}

void PreparedStatement::SetDouble(int index, double& value) {
	_ret = SQLBindParameter(
		_statement_handle,
		index,
		SQL_PARAM_INPUT,
		SQL_C_DOUBLE,
		SQL_DOUBLE, 0, 0, &value, 0, NULL);
	if ((_ret != SQL_SUCCESS) && (_ret != SQL_SUCCESS_WITH_INFO)) {
		ThrowException();
	}
}

void PreparedStatement::SetBigInt(int index, long long& value) {
	_ret = SQLBindParameter(
		_statement_handle,
		index,
		SQL_PARAM_INPUT,
		SQL_C_SBIGINT,
		SQL_BIGINT, 0, 0, &value, 0, NULL);
	if ((_ret != SQL_SUCCESS) && (_ret != SQL_SUCCESS_WITH_INFO)) {
		ThrowException();
	}
}

void PreparedStatement::SetString(int index, std::string& string) {
	_ret = SQLBindParameter(
		_statement_handle,
		index,
		SQL_PARAM_INPUT,
		SQL_C_CHAR,
		SQL_CHAR,
		string.length(),
		0,
		(SQLPOINTER)string.c_str(),
		0,
		&_nts_length);
	if ((_ret != SQL_SUCCESS) && (_ret != SQL_SUCCESS_WITH_INFO)) {
		ThrowException();
	}
}

void PreparedStatement::Close() {
	SQLCloseCursor(_statement_handle);
}

ResultSet* PreparedStatement::Execute() {
	_ret = SQLExecute(_statement_handle);
	if (_ret != SQL_SUCCESS) {
		ThrowException();
	}
	return new ResultSet(*this);
}

void PreparedStatement::ExecuteUpdate() {
	_ret = SQLExecute(_statement_handle);
	if ((_ret != SQL_SUCCESS) && (_ret != SQL_SUCCESS_WITH_INFO)) {
		ThrowException();
	}
}

SQLHSTMT PreparedStatement::GetStatementHandle()
{
	return _statement_handle;
}

void PreparedStatement::ThrowException() {
	int iDiag;
	SQLINTEGER NativeError;
	SQLCHAR SqlState[6];
	SQLCHAR Msg[255];
	SQLSMALLINT MsgLen;
	char str1[1024];
	char str2[1024];


	SQLGetDiagField(SQL_HANDLE_STMT, _statement_handle, 0, SQL_DIAG_NUMBER, &iDiag, 0, &MsgLen);
	sprintf_s(str1, 1024, "상태 레코드의 개수 = %d", iDiag);

	std::string err_msg("Exception) PreparedStatement - ");
	err_msg += str1;
	err_msg += '\n';

	for (iDiag = 1; ; iDiag++) {
		_ret = SQLGetDiagRec(SQL_HANDLE_STMT, _statement_handle, iDiag, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen);
		if (_ret == SQL_NO_DATA)
			break;
		sprintf_s(str2, 1024, "SQLSTATE:%s, NativeError:%d, 진단정보:%s", SqlState, NativeError, Msg);
		err_msg.append(str2);
	}
	throw std::logic_error(err_msg.c_str());
}
