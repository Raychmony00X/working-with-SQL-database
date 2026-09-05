#include "customer_managment.h"

#include <iostream>
#include <exception>

CustomerManagmentDatabase::CustomerManagmentDatabase() {
	m_connection = "host=localhost port=5432 user=postgres password=1488";
}

bool CustomerManagmentDatabase::CreateDatabase(const std::string& dbname) {
	try {
		pqxx::connection conn(m_connection);
		if (!conn.is_open()) return false;

		{
			pqxx::nontransaction tx(conn);
			tx.exec("DROP DATABASE IF EXISTS " + dbname);
			tx.exec("CREATE DATABASE " + dbname);
			tx.commit();
		}

		std::string new_connection = "host=localhost port=5432 dbname=" + dbname
			+ " user=postgres password=1488";
		pqxx::connection new_conn(new_connection);
		if (!new_conn.is_open()) return false;

		{
			pqxx::work tx(new_conn);
			tx.exec(R"( 
            CREATE TABLE IF NOT EXISTS customers_information (
               id SERIAL PRIMARY KEY,
               first_name VARCHAR(100) NOT NULL,
               second_name VARCHAR(100) NOT NULL,
               email TEXT UNIQUE
               )
			)");
			tx.exec(R"(
               CREATE TABLE IF NOT EXISTS customers_phones_numbers(
               phone_number_id SERIAL PRIMARY KEY,
               customer_id INTEGER REFERENCES customers_information(id) ON DELETE CASCADE,
               phone_number TEXT
               )
            )");
			tx.commit();
		}

	} catch (pqxx::sql_error& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool CustomerManagmentDatabase::AddNewCustomer(const std::string& dbname, 
	const std::string& first_name, const std::string& second_name, const 
	std::string& email, const std::string& phone_number) {
	try {
		std::string conn_str = m_connection + " dbname=" + dbname;
		pqxx::connection conn(conn_str);
		if (!conn.is_open()) return false;

		{
			pqxx::work tx(conn);
			pqxx::result result = tx.exec_params(
				"INSERT INTO customers_information(first_name, second_name, email) "
				"VALUES($1, $2, $3) RETURNING id", first_name, second_name, email);
			int customer_id = result[0][0].as<int>();
			tx.exec_params(
				"INSERT INTO customers_phones_numbers(customer_id, phone_number) "
				"VALUES($1, $2)", customer_id, phone_number);
			tx.commit();
		}

	} catch (pqxx::sql_error& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool CustomerManagmentDatabase::AddPhoneNumber(const std::string& dbname, const 
	std::string& phone_number, const std::string& email) {
	try {
		std::string conn_str = m_connection + " dbname=" + dbname;
		pqxx::connection conn(conn_str);
		if (!conn.is_open()) return false;

		{
			pqxx::work tx(conn);
			pqxx::result result = tx.exec_params("SELECT id FROM customers_information " 
				"WHERE email = $1", email);
			if (result.empty()) return false;
			int customer_id = result[0][0].as<int>();
			tx.exec_params("INSERT INTO customers_phones_numbers(customer_id, "
				"phone_number) VALUES($1, $2)", customer_id, phone_number);
			tx.commit();
		}

	} catch (pqxx::sql_error& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool CustomerManagmentDatabase::UpgradeCustomer(const std::string& dbname,
	const std::string& email, const std::string& first_name, const std::string& 
	second_name, const std::string& new_email, const std::string& phone_number) {
	try {
		std::string conn_str = m_connection + " dbname=" + dbname;
		pqxx::connection conn(conn_str);
		if (!conn.is_open()) return false;
		
		{
			pqxx::work tx(conn);
			pqxx::result result = tx.exec_params("SELECT id FROM customers_information "
				"WHERE email = $1", email);
			if (result.empty()) return false;
			int customer_id = result[0][0].as<int>();
			tx.exec_params("UPDATE customers_information SET first_name = $1, "
				"second_name = $2, email = $3 WHERE id = $4", first_name, second_name,
				new_email, customer_id);
			tx.exec_params("UPDATE customers_phones_numbers SET phone_number = $1 "
				"WHERE customer_id = $2", phone_number, customer_id);
			tx.commit();
		}

	} catch (pqxx::sql_error& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool CustomerManagmentDatabase::DeletePhoneNumber(const std::string& dbname, const
	std::string& phone_number, const std::string& email) {
	try {
		std::string conn_str = m_connection + " dbname=" + dbname;
		pqxx::connection conn(conn_str);
		if (!conn.is_open()) return false;

		{
			pqxx::work tx(conn);
			pqxx::result result = tx.exec_params("SELECT id FROM customers_information "
				"WHERE email = $1", email);
			if (result.empty()) return false;
			int customer_id = result[0][0].as<int>();
			tx.exec_params("DELETE FROM customers_phones_numbers WHERE customer_id = $1 "
				"AND phone_number = $2", customer_id, phone_number);
			tx.commit();
		}

	} catch (pqxx::sql_error& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool CustomerManagmentDatabase::DeleteCustomer(const std::string& dbname, const 
	std::string& email) {
	try {
		std::string conn_str = m_connection + " dbname=" + dbname;
		pqxx::connection conn(conn_str);
		if (!conn.is_open()) return false;

		{
			pqxx::work tx(conn);
			pqxx::result result = tx.exec_params("SELECT id FROM customers_information "
				"WHERE email = $1", email);
			if (result.empty()) return false;
			int customer_id = result[0][0].as<int>();
			tx.exec_params("DELETE FROM customers_information WHERE id = $1", customer_id);
			tx.commit();
		}

	}
	catch (pqxx::sql_error& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

Customer CustomerManagmentDatabase::FindCustomer(const std::string& dbname,
	const std::string& first_name, const std::string& second_name, const std::string& email) {
	Customer result; 

	try {
		std::string conn_str = m_connection + " dbname=" + dbname;
		pqxx::connection conn(conn_str);
		if (!conn.is_open()) return result;

		{
			pqxx::work tx(conn);
			pqxx::result customer_result = tx.exec_params(
				"SELECT id, first_name, second_name, email FROM customers_information "
				"WHERE first_name = $1 AND second_name = $2 AND email = $3",
				first_name, second_name, email);

			if (customer_result.empty()) return result;

			result.id = customer_result[0][0].as<int>();
			result.first_name = customer_result[0][1].as<std::string>();
			result.second_name = customer_result[0][2].as<std::string>();
			result.email = customer_result[0][3].as<std::string>();
			pqxx::result phone_result = tx.exec_params(
				"SELECT phone_number FROM customers_phones_numbers "
				"WHERE customer_id = $1",
				result.id);

			if (!phone_result.empty()) {
				result.phone_number = phone_result[0][0].as<std::string>();
			}

			tx.commit();
		}

	}
	catch (pqxx::sql_error& e) {
		std::cerr << e.what() << std::endl;
	}

	return result;
}