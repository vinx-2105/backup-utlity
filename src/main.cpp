#include "MySqlLiteConnector.hpp"
#include <iostream>

using namespace std; 

int main (){
    MySqlLiteConnector db_connector;
    time_t current_time;
    char* base_dt;
    current_time = time(NULL);
    base_dt = strtok(ctime(&current_time), "\n");;
    printf("base_dt: %s.....\n", base_dt);
    const char* src_path="/Users/vineet/Documents/cc/os/os-backup-utlity/build";
    const char* dest_path="/Users/vineet/Documents/cc/os/os-backup-utlity/build";
    const char* mode="full";
    const char* backup_status="init";
    const char* backup_status_2="completed";
    const int full_frequency=20;
    const int part_frequency=6;
    
    // db_connector.create_backup_config(src_path, dest_path, mode, base_dt, full_frequency, part_frequency);
    // db_connector.update_backup_config(1, src_path, dest_path, mode, base_dt, full_frequency, part_frequency);
    // db_connector.create_backup(1, base_dt, src_path, backup_status);
    // db_connector.update_backup(1, base_dt, backup_status_2);
    BackupConfig* bc; 
    Backup* b; 
    db_connector.get_backup_config(&bc, 1);
    db_connector.get_backup(&b, 6);
    printf("bc\n");
    bc->display();
    printf("b\n");
    b->display();
    vector<Backup*>* backups; 
    db_connector.get_backups_by_config(&backups, 1);
    for (vector<Backup*>::iterator it=backups->begin(); it!=backups->end(); it++){
        (*it)->display();
    }
}