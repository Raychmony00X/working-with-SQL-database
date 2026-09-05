#pragma once

#include <pqxx/pqxx>

struct Customer {
	int id;
	std::string first_name;
	std::string second_name;
	std::string email;
	std::string phone_number;
};

class CustomerManagmentDatabase {
public:
	explicit CustomerManagmentDatabase();

	bool CreateDatabase(const std::string& dbname);

	bool AddNewCustomer(const std::string& dbname, const std::string& first_name, 
		const std::string& second_name, const std::string& email, const 
		std::string& phone_number);

	bool AddPhoneNumber(const std::string& dbname, const std::string& phone_number, 
		const std::string& email);

	bool UpgradeCustomer(const std::string& dbname, const std::string& email, 
		const std::string& first_name, const std::string& second_name, const 
		std::string& new_email, const std::string& phone_number);

	bool DeletePhoneNumber(const std::string& dbname, const std::string& phone_number,
		const std::string& email);

	bool DeleteCustomer(const std::string& dbname, const std::string& email);

	Customer FindCustomer(const std::string& dbname, const std::string& first_name,
		const std::string& second_name, const std::string& email);

private:
	std::string m_connection;
};
