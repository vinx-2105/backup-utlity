#ifndef MYSQLLITE_CONNECTOR
#define MYSQLLITE_CONNECTOR
#include "MySqlLiteConnector.hpp"
#endif


#ifndef BACKUP_STATE
#define BACKUP_STATE
#include "BackupState.hpp"
#endif

#ifndef BACKUP_CONTROLLER
#define BACKUP_CONTROLLER
#include "BackupController.hpp"
#endif

#ifndef BACKUP_CONFIG
#define BACKUP_CONFIG
#include "BackupConfig.hpp"
#endif

#ifndef BACKUP
#define BACKUP
#include "Backup.hpp"
#endif

#ifndef TIME_UTILS
#define TIME_UTILS
#include "TimeUtils.hpp"
#endif

#ifndef BACKUP_CONTROLLER
#define BACKUP_CONTROLLER
#include "BackupController.hpp"
#endif

#include <iostream>

using namespace std; 

int main (){
    MySqlLiteConnector db_connector;
    const char* base_dt = curr_time_string();
    printf("base_dt: %s.....\n", base_dt);
    const char* src_path="/Users/vineet/Desktop";
    const char* dest_path="/Users/vineet/Desktop";
    const char* mode="full";
    // const char* backup_status="init";
    // const char* backup_status_2="completed";
    const int full_frequency=20;
    const int part_frequency=6;
    // const char* curr_time = curr_time_string();
    // db_connector.create_backup_config(src_path, dest_path, mode, base_dt, full_frequency, part_frequency);

    BackupController backup_controller(&db_connector);
    backup_controller.list_all_backup_configs();
    backup_controller.do_full_backup(1);
    // bs.load_from_file("a.json");
    // vector<tuple<string, string, int> > files; 
    // files.push_back(tuple<string, string, int>("file_path_here2", "datetime2_here", 3));
    // bs.save(1, "b.json", files);

}