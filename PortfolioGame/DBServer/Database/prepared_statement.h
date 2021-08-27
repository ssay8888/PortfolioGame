#pragma once
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
class ResultSet;
class Connection;

class PreparedStatement
{
public:
	PreparedStatement(const PreparedStatement&) = delete;
	PreparedStatement& operator=(const PreparedStatement&) = delete;
public:
	PreparedStatement(Connection& con);
	~PreparedStatement();
	void PrepareStatement(const char* string);
	void SetInt(int index, int value);
	void SetDouble(int index, double& value);
	void SetBigInt(int index, long long& value);
	void SetString(int index, std::string& string);
	void Close();

	// this method will delegate 'SQLCloseCursor' to ResultSet's destructor
	ResultSet* Execute();
	void		ExecuteUpdate();
	SQLHSTMT GetStatementHandle();
private:
	void ThrowException();
private:
	SQLRETURN _ret;
	SQLHSTMT _statement_handle;
	SQLLEN _nts_length;

	std::vector<char*> _binded;
};

