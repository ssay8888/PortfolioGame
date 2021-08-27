#include "pch.h"
#include "result_set.h"
#include "prepared_statement.h"
#include <sql.h>
#include <sqlext.h>

ResultSet::ResultSet(PreparedStatement& ps)
	: _ps(ps)
{}
ResultSet::~ResultSet()
{
}
bool ResultSet::Fetch() const
{
	const SQLRETURN ret = SQLFetch(_ps.GetStatementHandle());
	return ret != SQL_NO_DATA;
}
void ResultSet::Close() const
{
	SQLFreeStmt(_ps.GetStatementHandle(), SQL_UNBIND);
	SQLFreeStmt(_ps.GetStatementHandle(), SQL_RESET_PARAMS);
}
void ResultSet::GetInt(const int index, int* return_value) const
{
	SQLLEN value;
	SQLBindCol(_ps.GetStatementHandle(), index, SQL_C_SLONG, return_value, 0, &value);
}
void ResultSet::GetBigInt(const int index, long long* return_value) const
{
	SQLLEN value;
	SQLBindCol(_ps.GetStatementHandle(), index, SQL_C_SBIGINT, return_value, 0, &value);
}
void ResultSet::GetDouble(const int index, double* return_value) const
{
	SQLLEN value;
	SQLBindCol(_ps.GetStatementHandle(), index, SQL_C_DOUBLE, return_value, 0, &value);
}
void ResultSet::GetString(const int index, char* return_buffer, size_t size) const
{
	SQLLEN value;
	SQLBindCol(_ps.GetStatementHandle(), index, SQL_C_CHAR, return_buffer, size, &value);
}
