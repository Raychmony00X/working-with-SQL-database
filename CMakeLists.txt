cmake_minimum_required(VERSION 3.11) 
project(customer_managment) 
set(CMAKE_CXX_STANDARD 20) 


set(PQXX_INCLUDE_DIR "D:/vcpkg/installed/x64-windows/include")
set(PQXX_LIBRARY "D:/vcpkg/installed/x64-windows/lib/pqxx.lib")


include_directories(
    ${PQXX_INCLUDE_DIR}
    D:/vcpkg/installed/x64-windows/include
)


cmake_policy(SET CMP0074 NEW)

find_package(PostgreSQL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp) 


target_include_directories(${PROJECT_NAME} PRIVATE 
    ${PQXX_INCLUDE_DIR}
    D:/vcpkg/installed/x64-windows/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    PostgreSQL::PostgreSQL 
    ${PQXX_LIBRARY}
)