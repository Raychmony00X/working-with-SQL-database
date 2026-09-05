#include "customer_managment.h"
#include <iostream>

int main() {
    CustomerManagmentDatabase db;
    const std::string TEST_DB = "test_db";

    std::cout << "=== TEST 1: Create Database ===" << std::endl;
    bool test1 = db.CreateDatabase(TEST_DB);
    std::cout << (test1 ? "PASS" : "FAIL") << std::endl << std::endl;

    std::cout << "=== TEST 2: Add Customer ===" << std::endl;
    bool test2 = db.AddNewCustomer(TEST_DB, "John", "Doe", "john@email.com", "+123456789");
    std::cout << (test2 ? "PASS" : "FAIL") << std::endl << std::endl;

    std::cout << "=== TEST 3: Find Customer ===" << std::endl;
    Customer found = db.FindCustomer(TEST_DB, "John", "Doe", "john@email.com");
    if (found.id != 0) {
        std::cout << "PASS - Customer found: " << found.first_name << " " << found.second_name << std::endl;
    }
    else {
        std::cout << "FAIL - Customer not found" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "=== TEST 4: Add Phone ===" << std::endl;
    bool test4 = db.AddPhoneNumber(TEST_DB, "+987654321", "john@email.com");
    std::cout << (test4 ? "PASS" : "FAIL") << std::endl << std::endl;

    std::cout << "=== TEST 5: Update Customer ===" << std::endl;
    bool test5 = db.UpgradeCustomer(TEST_DB, "john@email.com", "Jonathan", "Doe", "jonathan@email.com", "+111111111");
    std::cout << (test5 ? "PASS" : "FAIL") << std::endl << std::endl;

    std::cout << "=== TEST 6: Delete Phone ===" << std::endl;
    bool test6 = db.DeletePhoneNumber(TEST_DB, "+987654321", "jonathan@email.com");
    std::cout << (test6 ? "PASS" : "FAIL") << std::endl << std::endl;

    std::cout << "=== TEST 7: Delete Customer ===" << std::endl;
    bool test7 = db.DeleteCustomer(TEST_DB, "jonathan@email.com");
    std::cout << (test7 ? "PASS" : "FAIL") << std::endl << std::endl;

    std::cout << "=== ALL TESTS COMPLETED ===" << std::endl;

    return 0;
}