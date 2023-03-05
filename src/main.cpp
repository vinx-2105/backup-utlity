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
    //driver program
    //1. create a new backup config
    //2. display all backup_configs
    //3. display all backups given a backup_config_id
    //4. do a full backup while inputing a  backup_config_id
    //5. resume a previous backup which was not completed before
    //6. incremental backup - backs up only those files which have been changed since the last full backup

    // const char* src_path="/Users/vineet/Desktop";///these params in backup config are all hardcoded for now
    // const char* dest_path="/Users/vineet/Desktop";
    const char* mode="full";
    const int full_frequency=20;
    const int part_frequency=6;

    MySqlLiteConnector db_connector;
    BackupController bc(&db_connector); 

    while (true){
        printf("\nSelect a command...\n");
        int mode;
        printf("<1> Create a new backup config\n");
        printf("<2> Display all backup configs\n");
        printf("<3> Display all backups given a backup_config_id\n");
        printf("<4> Take a full backup given a valid backup_config_id\n");
        printf("<5> Resume a previously incompleted backup\n");
        printf("<6> Take a incremental backup given a backup_config_id\n");

        cin>>mode; 

        if (mode==1){
            printf("\tSpecify the path of the dir to be backed up (like /Users/vineet/Desktop): \n");
            string src_path; 
            cin>>src_path; 
            db_connector.create_backup_config(src_path.c_str(), src_path.c_str(), "full", curr_time_string().c_str(), full_frequency, part_frequency);
        }
        else if(mode==2){
            printf("\tAll backup configs:\n");
            bc.list_all_backup_configs();
        }
        else if (mode==3){
            printf("\tSpecify the backup_config_id:\n");
            int backup_config_id; 
            cin>>backup_config_id;
            bc.list_all_backups_by_backup_config_id(backup_config_id);
            
        }
        else if (mode==4){//take a full backup given a backup_config_id
            printf("\tSpecify the backup_config_id:\n");
            int backup_config_id; 
            cin>>backup_config_id;
            bc.do_full_backup(backup_config_id);
        }
        else if (mode==5){
            printf("\tSpecify the backup_id:\n");
            int backup_id; 
            cin>>backup_id; 
            bc.resume_backup(backup_id);
        }
        else if(mode==6){
            printf("\tSpecify the backup_config_id:\n");
            int backup_config_id; 
            cin>>backup_config_id;
            bc.do_incremental_backup(backup_config_id);
        }
        else {
            printf("Invalid command selected...");
            continue;
        }
    }
}