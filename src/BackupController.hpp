#ifndef ALGORITHM
#define ALGORITHM
#include <algorithm>
#endif

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

#ifndef TIME_UTILS
#define TIME_UTILS
#include "TimeUtils.hpp"
#endif


#ifndef FILESYSTEM
#define FILESYSTEM
#include <filesystem>
#endif




/*This is the main driver class*/
class BackupController {
    private: 
        BackupDatabase* b_db; 
    public: 
        BackupController(BackupDatabase* b_db){
            this->b_db = b_db; 
        };

        //list all backup configs
        int list_all_backup_configs(){
            vector<BackupConfig*> *all_configs; 
            this->b_db->get_all_backup_configs(&all_configs);//check for the error handling code
            for (vector<BackupConfig*>::iterator it=all_configs->begin(); it!=all_configs->end(); it++){
                (*it)->display();
            }
            return 0;
        }

        //run a full backup given a config_id
        int do_full_backup(int backup_config_id){
            //find the config first
            BackupConfig* backup_config; 
            if (this->b_db->get_backup_config(&backup_config, backup_config_id)){
                return -1;
            }
            backup_config->display();
            //validate the backup config to check if the paths actualy exist
            if (!backup_config->validate_src()){
                printf("Backup config src is invalid\n");
            }
            if (!std::filesystem::exists(backup_config->get_dest_path())){
                printf("Backup config dest path does not exist...creating new dir...\n");
                if (std::filesystem::create_directory(backup_config->get_dest_path())){
                    return -1;
                }
                else {
                    printf("Backup config dest path created...\n");
                }
            }
            //create the backup path dir && initiate the backup obj in db
            string start_time(curr_time_string());//from TimeUtils
            string file_time(start_time);//from TimeUtils
            std::replace(file_time.begin(), file_time.end(), ' ', '_');
            string backup_root_path(backup_config->get_dest_path());
            printf("backup_root_path:::%s\n", backup_root_path.c_str());
            backup_root_path+="/";
            printf("backup_root_path:::%s\n", backup_root_path.c_str());
            backup_root_path+=file_time;
            printf("backup_root_path:::%s\n", backup_root_path.c_str());
            std::filesystem::create_directory(backup_root_path.c_str());
            int backup_id = b_db->create_backup(backup_config_id, start_time.c_str(), backup_root_path.c_str());
            if (backup_id==-1){//handle error
                return -1;
            }
            //find all the files in src_path and create the backup state
            printf("Creating backup state...\n");
            BackupState bs;
            string backup_state_path(backup_root_path);
            backup_state_path+="state.json";
            vector<tuple<const char*, const char*, int> > files;
            unsigned int file_count = 0;
            //TODO: limited to a simple dir for now
            for (const std::filesystem::directory_entry& f : std::filesystem::directory_iterator(backup_config->get_src_path())){
                if (f.is_regular_file()){
                    file_count++;
                    const char* fpath = f.path().c_str();
                    if (strlen(fpath)>0 && fpath[0]!='.'){
                        const char* fname = std::filesystem::path(fpath).filename().c_str();
                        string dest_fpath(backup_root_path+"/");
                        dest_fpath+=fname;
                        const char* curr_time = file_time_to_time_string(f.last_write_time().max());
                        std::filesystem::copy_file(fpath, dest_fpath);
                        files.push_back(tuple<const char*, const char*, int> (fpath, curr_time, backup_id));
                    }
                    printf("file_count:::%d\n", file_count);
                }
            }
            printf("file_count_final:::%d\n", file_count);
            bs.save_new(backup_id, backup_state_path.c_str(), files); 
            printf("do_full_backup::::after state file save");
            b_db->update_backup(backup_id, curr_time_string(), "completed");
            return 0;
        }
};