#include "../pch.h"
#include "database_connector.h"
#include "connection.h"
#include <sqlext.h>

DatabaseConnector::DatabaseConnector(const char* db_name, const char* id, const char* pw) {
	SetData(db_name, id, pw);
}

DatabaseConnector::~DatabaseConnector() {}

void DatabaseConnector::SetData(const char* db_name, const char* id, const char* pw) {
	strncpy_s(reinterpret_cast<char*>(_data_source_name), 128, db_name, strlen(db_name));
	strncpy_s(reinterpret_cast<char*>(_id), 64, id, strlen(id));
	strncpy_s(reinterpret_cast<char*>(_pw), 64, pw, strlen(pw));
}

void DatabaseConnector::Init()
{
	if (SQLAllocHandle(SQL_HANDLE_ENV, nullptr, &_environment_handle) != SQL_SUCCESS)
	{
		throw std::logic_error("Exception) SQL Enviroment handle allocation Fail...");
	}
	if (SQLSetEnvAttr(_environment_handle, SQL_ATTR_ODBC_VERSION,
	                  reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		throw std::logic_error("Exception) SQL Enviroment handle sets attributes Fail...");
	}
}

Connection& DatabaseConnector::GetConnection() {
	static thread_local Connection con(*this);
	return con;
}

DatabaseConnector* DatabaseConnector::GetInstance()
{
	static DatabaseConnector instance;
	return &instance;
}

SQLCHAR* DatabaseConnector::GetDataSourceName()
{
	return _data_source_name;
}
SQLCHAR* DatabaseConnector::GetId()
{
	return _id;
}
SQLCHAR* DatabaseConnector::GetPw()
{
	return _pw;
}
SQLHENV DatabaseConnector::GetEnvironmentHandle() const
{
	return _environment_handle;
}
