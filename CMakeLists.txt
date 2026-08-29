cmake_minimum_required(VERSION 3.20)
project(Customer)
add_executable(Customer main.cpp)

set(PostgreSQL_ADDITIONAL_VERSIONS "18")

add_subdirectory("D:\libpqxx\libpqxx-8.0.2" libpqxx-build)

target_compile_features(Customer PRIVATE cxx_std_17)

target_link_libraries(Customer pqxx)