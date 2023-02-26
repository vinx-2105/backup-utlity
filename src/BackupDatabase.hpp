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


class BackupDatabase {
    public:
        virtual int create_backup_config(const char* src_path, const char* dest_path, const char* mode, const char* base_dt, const int full_frequency, const int part_frequency)=0;
        virtual void update_backup_config(const int id, const char* src_path, const char* dest_path, const char* mode, const char* base_dt, const int full_frequency, const int part_frequency)=0;
        virtual int create_backup(const int backup_config_id, const char* start_dt, const char* path, const char* status)=0;
        virtual void update_backup(const int backup_id, const char* end_dt, const char* status)=0;
        virtual int get_all_backup_configs(vector<BackupConfig*>** all_configs)=0;
        virtual int get_backup_config(BackupConfig** backup_config, int id)=0;
        virtual int get_backup(Backup** backup, int id)=0;
        virtual int get_backups_by_config(vector<Backup*>** backups, int backup_config_id)=0;
};