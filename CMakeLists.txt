cmake_minimum_required(VERSION 3.0.0)

project(BackupUtility VERSION 1.0 DESCRIPTION "A utility for creating file backups" LANGUAGES CXX)
set(TARGET_NAME "BackupUtility")
find_library(SQLITE3 sqlite3)
message(${SQLITE3})
target_link_libraries(${TARGET_NAME} PUBLIC ${SQLITE3})
target_include_directories(${TARGET_NAME} PUBLIC /usr/local/include/sqlite3/)
add_executable(TARGET_NAME  ${SQLITE3} src/BackupDatabase.hpp src/MySqlLiteConnector.hpp src/main.cpp)
