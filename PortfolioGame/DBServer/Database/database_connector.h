#pragma once
#include <sqltypes.h>
class Connection;

class DatabaseConnector
{
public:
	DatabaseConnector() = default;
	DatabaseConnector(const char* db_name, const char* id, const char* pw);
	~DatabaseConnector();
	void SetData(const char* db_name, const char* id, const char* pw);
	void Init();
	Connection& GetConnection();
	static DatabaseConnector* GetInstance();

	SQLCHAR* GetDataSourceName();
	SQLCHAR* GetId();
	SQLCHAR* GetPw();
	SQLHENV& GetEnvironmentHandle();
private:
	SQLCHAR _data_source_name[128];
	SQLCHAR _id[64];
	SQLCHAR _pw[64];
	SQLHENV _environment_handle;
};

