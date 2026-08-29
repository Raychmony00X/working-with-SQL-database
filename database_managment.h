#pragma once

#include <pqxx/pqxx>

#include <string>

class DatabaseManagment {
public:
	explicit DatabaseManagment(const std::string& connection_string);

	bool CreateDatabase(const std::string& dbname);

	bool ExistsDatabase(const std::string& dbname);

	bool ToConnect(const std::string& dbname);

	bool ExecuteQuery(const std::string& query);

	pqxx::result GetQueryResult(const std::string& query);

private:
	pqxx::connection* m_connection;
	std::string m_connection_string;
	bool m_connected;
};