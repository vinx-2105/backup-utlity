#ifndef BACKUP_CONFIG
#define BACKUP_CONFIG
#include "BackupConfig.hpp"
#endif

#ifndef BACKUP
#define BACKUP
#include "Backup.hpp"
#endif

#ifndef BACKUP_DATABASE
#define BACKUP_DATABASE
#include "BackupDatabase.hpp"
#endif

class BackupController {
    private: 
        BackupDatabase* b_db; 
    public: 
        BackupController(BackupDatabase* b_db){
            this->b_db = b_db; 
        };
};