#include <sqlite3.h>
#include <stdio.h>
#include "BackupDatabase.hpp"

class MySqlLiteConnector : public BackupDatabase {
    private: 
        sqlite3* db;
        static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
            int i;
            for(i = 0; i<argc; i++) {
                printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
            }
            printf("\n");
            return 0;
        }
    public: 
        MySqlLiteConnector(){
            int res = sqlite3_open("test.db", &this->db);
            char *errMsg=0; 
            if(res) {
                fprintf(stderr, "Could not open database: %s\n", sqlite3_errmsg(db));
            } 
            else {
                printf("Successfully connected to the database");
            }
            const char* init_sql_statement = " "\
                "CREATE DATABASE IF NOT EXISTS Backup_Info; "\
                "CREATE TABLE IF NOT EXISTS Backup_Configs ("\
                    "id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,"\
                    "backup_src_path CHAR(255) NOT NULL,"\
                    "backup_dest_path CHAR(255) NOT NULL,"\
                    "mode ENUM('full','differential','incremental','continuous') NOT NULL,"\
                    "base_dt DATETIME NOT NULL,"\
                    "full_frequency INTEGER NOT NULL,"\
                    "part_frequency INTEGER NOT NULL,"\
                    "deleted_at DATETIME"\
                ");"\
                "CREATE TABLE IF NOT EXISTS Backups ("\
                    "id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,"\
                    "backup_config_id INT NOT NULL,"\
                    "start_dt datetime,"\
                    "end_dt datetime,"\
                    "path CHAR(255) NOT NULL,"\
                    "status ENUM('init','ongoing','failed','completed') NOT NULL DEFUALT 'init'"\
                ");"\
                "";
            res = sqlite3_exec(this->db, init_sql_statement, this->callback, 0, &errMsg);
            if(res!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errMsg);
                sqlite3_free(errMsg);
            } else {
                printf("Database and tables created");
            }
            sqlite3_close(this->db);
        };
        
        ~MySqlLiteConnector(){ };

        void create_backup() {

        };
        void update_backup() {

        };
        void create_backup_config() {

        };
        void update_backup_config() {

        };
        void delete_backup_config() {

        };
};