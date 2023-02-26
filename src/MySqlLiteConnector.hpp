#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <vector>
#ifndef BACKUP_CONFIG
#define BACKUP_CONFIG
#else 
#include "BackupConfig.hpp"
#endif
#ifndef BACKUP
#define BACKUP
#else 
#include "Backup.hpp"
#endif
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
                printf("Successfully connected to the database\n");
            }
            //ENUM('full','differential','incremental','continuous') NOT NULL
            //ENUM('init','ongoing','failed','completed') NOT NULL DEFUALT 'init'
            const char* init_sql_statement = "CREATE TABLE IF NOT EXISTS Backup_Configs ("\
                    "id INTEGER PRIMARY KEY NOT NULL,"\
                    "src_path CHAR(255) NOT NULL,"\
                    "dest_path CHAR(255) NOT NULL,"\
                    "mode CHAR(255) NOT NULL,"\
                    "base_dt DATETIME NOT NULL,"\
                    "full_frequency INTEGER NOT NULL,"\
                    "part_frequency INTEGER NOT NULL,"\
                    "deleted_at DATETIME"\
                ");"\
                "CREATE TABLE IF NOT EXISTS Backups ("\
                    "id INTEGER PRIMARY KEY NOT NULL,"\
                    "backup_config_id INTEGER NOT NULL,"\
                    "start_dt datetime,"\
                    "end_dt datetime,"\
                    "path CHAR(255) NOT NULL,"\
                    "status CHAR(255) NOT NULL"\
                ");"\
                "";
            res = sqlite3_exec(this->db, init_sql_statement, this->callback, 0, &errMsg);
            if(res!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errMsg);
                sqlite3_free(errMsg);
            } else {
                printf("Database and tables created\n");
            }
            sqlite3_close(this->db);
        };
        
        ~MySqlLiteConnector(){ };
        //  todo: list all backup configs
        
        int create_backup_config(const char* src_path, const char* dest_path, const char* mode, const char* base_dt, const int full_frequency, const int part_frequency) {
            int res = sqlite3_open("test.db", &this->db);
            char *errMsg=0; 
            char query[1000];
            sprintf(query, "INSERT INTO Backup_Configs (src_path, dest_path, mode, base_dt, full_frequency, part_frequency) VALUES ('%s', '%s', '%s', '%s', '%d', '%d')", src_path, dest_path, mode, base_dt, full_frequency, part_frequency);
            res = sqlite3_exec(this->db, query, this->callback, 0, &errMsg);
            if(res!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errMsg);
                sqlite3_free(errMsg);
            } else {
                printf("Created backup config\n");
            }
            int row_id = sqlite3_last_insert_rowid(this->db);
            sqlite3_close(this->db);
            return row_id; 
        };

        void update_backup_config(const int id, const char* src_path, const char* dest_path, const char* mode, const char* base_dt, const int full_frequency, const int part_frequency) {
            int res = sqlite3_open("test.db", &this->db);
            char *errMsg=0; 
            char query[1000];
            sprintf(query, "UPDATE Backup_Configs SET src_path='%s', dest_path='%s', mode='%s', base_dt='%s', full_frequency='%d', part_frequency='%d' WHERE id=%d", src_path, dest_path, mode, base_dt, full_frequency, part_frequency, id);
            res = sqlite3_exec(this->db, query, this->callback, 0, &errMsg);
            if(res!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errMsg);
                sqlite3_free(errMsg);
            } else {
                printf("Updated backup config\n");
            }
            sqlite3_close(this->db);
        };

        int get_all_backup_configs(vector<BackupConfig*>** configs){
            int res = sqlite3_open("test.db", &this->db);
            char *errMsg=0; 
            char query[1000];
            int num_cols = 8;
            int row = 0;
            int bytes[num_cols]; 
            sqlite3_stmt* stmt;
            const unsigned char* text[num_cols];
            *configs = new vector<BackupConfig*>();
            sprintf(query, "SELECT * FROM Backup_Configs");
            sqlite3_prepare(this->db, query, sizeof(query), &stmt, NULL);
            bool done = false;
            while (!done) {
                switch (sqlite3_step (stmt)) {
                    case SQLITE_ROW:{
                        char const* s1 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 1));
                        char const* s2 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 2));
                        char const* s3 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 3));
                        char const* s4 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 4));
                        char const* s7 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 7));

                        BackupConfig* bc = new BackupConfig(
                            sqlite3_column_int(stmt, 0),
                            s1, s2, s3, s4,
                            sqlite3_column_int(stmt, 5),
                            sqlite3_column_int(stmt, 6),
                            s7
                        );
                        (*configs)->push_back(bc);
                        row++;
                        break;
                    }
                    case SQLITE_DONE:
                        done = true;
                        break;

                    default:
                        fprintf(stderr, "Failed.\n");
                        return -1;
                }
            }
            sqlite3_finalize(stmt);
            sqlite3_close(this->db);
            return 0;
        }

        int get_backup_config(BackupConfig** backup_config, int id){
            int res = sqlite3_open("test.db", &this->db);
            char *errMsg=0; 
            char query[1000];
            int num_cols = 8;
            int row = 0;
            int bytes[num_cols]; 
            sqlite3_stmt* stmt;
            const unsigned char* text[num_cols];
            sprintf(query, "SELECT * FROM Backup_Configs WHERE id=%d", id);
            sqlite3_prepare(this->db, query, sizeof(query), &stmt, NULL);
            bool done = false;
            while (!done) {
                switch (sqlite3_step (stmt)) {
                    case SQLITE_ROW:{
                        char const* s1 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 1));
                        char const* s2 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 2));
                        char const* s3 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 3));
                        char const* s4 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 4));
                        char const* s7 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 7));

                        *backup_config = new BackupConfig(
                            sqlite3_column_int(stmt, 0),
                            s1, s2, s3, s4,
                            sqlite3_column_int(stmt, 5),
                            sqlite3_column_int(stmt, 6),
                            s7
                        );
                        return 0;
                        break;
                    }
                    case SQLITE_DONE:
                        done = true;
                        break;

                    default:
                        fprintf(stderr, "Failed.\n");
                        return -1;
                }
            }
            sqlite3_finalize(stmt);
            sqlite3_close(this->db);
            return -1;
        }

        int create_backup(const int backup_config_id, const char* start_dt, const char* path, const char* status) {
            int res = sqlite3_open("test.db", &this->db);
            char *errMsg=0; 
            char query[1000];
            sprintf(query, "INSERT INTO Backups (backup_config_id, start_dt, path, status) VALUES ('%d', '%s', '%s', '%s')", backup_config_id, start_dt, path, status);
            res = sqlite3_exec(this->db, query, this->callback, 0, &errMsg);
            if(res!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errMsg);
                sqlite3_free(errMsg);
            } else {
                printf("Created backup in db\n");
            }
            int row_id = sqlite3_last_insert_rowid(this->db);
            sqlite3_close(this->db);
            return row_id; 
        };

        void update_backup(const int backup_id, const char* end_dt, const char* status) {
            int res = sqlite3_open("test.db", &this->db);
            char *errMsg=0; 
            char query[1000];
            if (end_dt!=NULL){
                sprintf(query, "UPDATE Backups SET end_dt='%s', status='%s' WHERE id=%d", end_dt, status, backup_id);
            }
            else {
                sprintf(query, "UPDATE Backups SET status='%s' WHERE id=%d", status, backup_id);
            }
            res = sqlite3_exec(this->db, query, this->callback, 0, &errMsg);
            if(res!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", errMsg);
                sqlite3_free(errMsg);
            } else {
                printf("Updated backup in db\n");
            }
            sqlite3_close(this->db);
        };

        int get_backup(Backup** backup, int id){
            int res = sqlite3_open("test.db", &this->db);
            char *errMsg=0; 
            char query[1000];
            int num_cols = 6;
            int row = 0;
            int bytes[num_cols]; 
            sqlite3_stmt* stmt;
            const unsigned char* text[num_cols];
            sprintf(query, "SELECT * FROM Backups WHERE id=%d", id);
            sqlite3_prepare(this->db, query, sizeof(query), &stmt, NULL);
            bool done = false;
            while (!done) {
                switch (sqlite3_step (stmt)) {
                    case SQLITE_ROW:{
                        char const* s2 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 2));
                        char const* s3 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 3));
                        char const* s4 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 4));
                        char const* s5 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 5));

                        *backup = new Backup(
                            sqlite3_column_int(stmt, 0),
                            sqlite3_column_int(stmt, 1),
                            s2, s3, s4, s5
                        );
                        return 0;
                        break;
                    }
                    case SQLITE_DONE:
                        done = true;
                        break;

                    default:
                        fprintf(stderr, "Failed.\n");
                        return -1;
                }
            }
            sqlite3_finalize(stmt);
            sqlite3_close(this->db);
            return -1;
        }

        int get_backups_by_config(vector<Backup*>** backups, int backup_config_id){
            int res = sqlite3_open("test.db", &this->db);
            char *errMsg=0; 
            char query[1000];
            int num_cols = 6;
            int row = 0;
            int bytes[num_cols]; 
            sqlite3_stmt* stmt;
            *backups = new vector<Backup*>();
            const unsigned char* text[num_cols];
            sprintf(query, "SELECT * FROM Backups WHERE backup_config_id=%d", backup_config_id);
            sqlite3_prepare(this->db, query, sizeof(query), &stmt, NULL);
            bool done = false;
            while (!done) {
                switch (sqlite3_step (stmt)) {
                    case SQLITE_ROW:{
                        char const* s2 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 2));
                        char const* s3 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 3));
                        char const* s4 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 4));
                        char const* s5 = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 5));
                        printf("s2: %s\n", s2);
                        printf("s3: %s\n", s3);
                        printf("s4: %s\n", s4);
                        printf("s5: %s\n", s5);
                        Backup *backup = new Backup(
                            sqlite3_column_int(stmt, 0),
                            sqlite3_column_int(stmt, 1),
                            s2, s3, s4, s5
                        );
                        (*backups)->push_back(backup);
                        row++;
                        break;
                    }
                    case SQLITE_DONE:
                        done = true;
                        break;

                    default:
                        fprintf(stderr, "Failed.\n");
                        return -1;
                }
            }
            sqlite3_finalize(stmt);
            sqlite3_close(this->db);
            return 0;
        }

};