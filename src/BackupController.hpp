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

        //list all backups given a config id
        int list_all_backups_by_backup_config_id(int backup_config_id){
            vector<Backup*> *backups; 
            this->b_db->get_backups_by_config(&backups, backup_config_id);//check for the error handling code
            for (vector<Backup*>::iterator it=backups->begin(); it!=backups->end(); it++){
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
            string start_time = curr_time_string();//from TimeUtils
            string file_time(start_time);//from TimeUtils
            // std::replace(file_time.begin(), file_time.end(), ' ', '_');
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
            backup_state_path+="/state.json";
            vector<tuple<string, string, int> > files;
            for (const std::filesystem::directory_entry& f : std::filesystem::directory_iterator(backup_config->get_src_path())){
                b_db->update_backup(backup_id, curr_time_string().c_str(), "ongoing");
                if (f.is_regular_file()){
                    const char* fpath = f.path().c_str();
                    if (strlen(fpath)>0 && fpath[0]!='.'){
                        string fname = std::filesystem::path(fpath).filename();
                        if (fname[0]=='.') continue; 
                        string dest_fpath(backup_root_path+"/");
                        dest_fpath+=fname;
                        string curr_time = file_time_to_time_string(f.last_write_time());
                        std::filesystem::copy_file(fpath, dest_fpath);
                        files.push_back(tuple<string, string, int> (fname, curr_time, backup_id));
                        bs.write_to_file(backup_id, backup_state_path.c_str(), files, "ongoing"); 
                    }
                }
            }
            bs.write_to_file(backup_id, backup_state_path.c_str(), files, "completed"); 
            b_db->update_backup(backup_id, curr_time_string().c_str(), "completed");
            return 0;
        }

        int find_the_last_full_backup(Backup** backup, int backup_config_id){
            //find all backups of the backup_config and then sort (backup has to be complete)
            //caller will need to check if the backup pointer is non-null
            vector<Backup*>* all_backups; 
            if (this->b_db->get_backups_by_config(&all_backups, backup_config_id)){
                return -1;
            }
            //sort the backups 
            sort(all_backups->begin(), all_backups->end(), [](Backup* a, Backup* b){ return (a->get_id()<b->get_id()); });//assuming IDs are auto-increment 
            //get the last backup which has been completed
            for (int i=all_backups->size()-1; i>=0; i--){
                if ((*all_backups)[i]->get_status()=="completed"){
                    *backup = (*all_backups)[i];
                    return 0;
                } 
            }
            return 0;
        }

        int find_ongoing_backups(vector<int>** ongoing_backup_ids, int backup_config_id){
            BackupConfig* backup_config; 
            if (this->b_db->get_backup_config(&backup_config, backup_config_id)){
                return -1;
            }
            //check if in the DB, there are any backups with ongoing status
            vector<Backup*>* backups_by_config = new vector<Backup*>();
            this->b_db->get_backups_by_config(&backups_by_config, backup_config_id);
            //check the status of the all the backups and if anyone of the backups is still ongoing
            printf("find_ongoing_backups:::found %lu backups for this backup_config_id: %d\n", backups_by_config->size(), backup_config_id);
            //check the number of backups among all these which are ongoing
            vector<Backup*> ongoing_backups;
            for (vector<Backup*>::iterator it=backups_by_config->begin(); it!=backups_by_config->end(); it++){
                //check if the backup status is ongoing
                if((*it)->get_status()=="ongoing"){
                    ongoing_backups.push_back(*it);
                    (*ongoing_backup_ids)->push_back((*it)->get_id()); 
                }
            }
            printf("find_ongoing_backups:::found %lu ongoing backups\n", ongoing_backups.size());
            printf("find_ongoing_backups:::found %lu ongoing backup ids\n", (*ongoing_backup_ids)->size());
            return 0;
        }

        int resume_backup(int backup_id){
            //find the backup_config
            BackupConfig* backup_config; 
            //find the backup info from the db 
            Backup *backup; 
            if (this->b_db->get_backup(&backup, backup_id)){    
                return -1; 
            }
            //get the  backup config id
            int backup_config_id = backup->get_backup_config_id();
            //get the backup config id
            if (this->b_db->get_backup_config(&backup_config, backup_config_id)){
                return -1; 
            }
            printf("resume_backup::\n");            
            printf("resume_backup::backup obj:::::");
            backup->display();
            string backup_root_path = backup->get_root_path();
            string backup_state_path = backup->get_state_file_path();
            printf("resume_backup::backup_root_path::: %s\n", backup_root_path.c_str());            
            printf("resume_backup::backup_state_file_path::: %s\n", backup_state_path.c_str());
            printf("resume_backup::backup_config_id::: %d\n", backup_config_id);

            BackupState bs(backup_root_path.c_str());
            printf("HEREEEEEE......111:::\n");
            vector<tuple<string, string, int> > files=bs.get_files();
            map<string, string> file_name_to_modified_at_map = bs.get_file_path_to_datetime_map();
            map<string, int> file_name_to_backup_id_map = bs.get_file_path_to_backup_id_map();
            printf("HEREEEEEE......\n");
            for (const std::filesystem::directory_entry& f : std::filesystem::directory_iterator(backup_config->get_src_path())){
                b_db->update_backup(backup_id, curr_time_string().c_str(), "ongoing");
                if (f.is_regular_file()){
                    const char* fpath = f.path().c_str();
                    if (strlen(fpath)>0 && fpath[0]!='.'){
                        string fname = std::filesystem::path(fpath).filename();
                        if (fname[0]=='.') continue; 
                        string dest_fpath(backup_root_path+"/");
                        dest_fpath+=fname;
                        string curr_time = file_time_to_time_string(f.last_write_time());
                        if (file_name_to_modified_at_map.find(fname)==file_name_to_modified_at_map.end()){//this file was not present in the state file
                            std::filesystem::copy_file(fpath, dest_fpath);
                            files.push_back(tuple<string, string, int> (fname, curr_time, backup_id));
                            bs.write_to_file(backup_id, backup_state_path.c_str(), files, "ongoing"); 
                        }
                    }
                }
            }
            bs.write_to_file(backup_id, backup_state_path.c_str(), files, "completed"); 
            b_db->update_backup(backup_id, curr_time_string().c_str(), "completed");
            return 0;
        }

        int do_incremental_backup(int backup_config_id){
            Backup* last_full_backup=NULL;
            BackupConfig* backup_config=NULL; 
            //find the latest full backup
            if (this->find_the_last_full_backup(&last_full_backup, backup_config_id)){
                return -1; 
            };
            if (last_full_backup==NULL){
                return -1;
            }
            if (this->b_db->get_backup_config(&backup_config, backup_config_id)){
                return -1; 
            }
            //display the last_full_backup
            printf("do_incremental_backup::::last_full_backup\n");
            last_full_backup->display();
            int last_full_backup_id = last_full_backup->get_id();
            //load the state from the last full backup
            BackupState full_backup_bs(last_full_backup->get_state_file_path());
            map<string, string> full_backup_file_name_to_modified_at_map = full_backup_bs.get_file_path_to_datetime_map();
            map<string, int> full_backup_file_name_to_backup_id_map = full_backup_bs.get_file_path_to_backup_id_map();
            //start the new backup (incremental)
            if (!backup_config->validate_src()){
                printf("Backup config src is invalid\n");
            }
            if (!std::filesystem::exists(backup_config->get_dest_path())){
                printf("Backup config dest path does not exist...last full backup also be corrupt\n");
                return -1;
            }
            //create the backup path dir && initiate the backup obj in db
            string start_time = curr_time_string();//from TimeUtils
            string file_time(start_time);//from TimeUtils
            // std::replace(file_time.begin(), file_time.end(), ' ', '_');
            string backup_root_path(backup_config->get_dest_path());
            printf("do_incremental_backup:::backup_root_path:::%s\n", backup_root_path.c_str());
            backup_root_path+="/";
            printf("do_incremental_backup:::backup_root_path:::%s\n", backup_root_path.c_str());
            backup_root_path+=file_time;
            printf("do_incremental_backup:::backup_root_path:::%s\n", backup_root_path.c_str());
            std::filesystem::create_directory(backup_root_path.c_str());
            int backup_id = b_db->create_backup(backup_config_id, start_time.c_str(), backup_root_path.c_str());
            if (backup_id==-1){//handle error
                return -1;
            }
            //find all the files in src_path and create the backup state
            printf("Creating backup state...\n");
            BackupState bs;
            string backup_state_path(backup_root_path);
            backup_state_path+="/state.json";
            vector<tuple<string, string, int> > files;
            for (const std::filesystem::directory_entry& f : std::filesystem::directory_iterator(backup_config->get_src_path())){
                b_db->update_backup(backup_id, curr_time_string().c_str(), "ongoing");
                if (f.is_regular_file()){
                    const char* fpath = f.path().c_str();
                    if (strlen(fpath)>0 && fpath[0]!='.'){
                        string fname = std::filesystem::path(fpath).filename();
                        if (fname[0]=='.') continue; 
                        string dest_fpath(backup_root_path+"/");
                        dest_fpath+=fname;
                        string curr_time = file_time_to_time_string(f.last_write_time());
                        if ((full_backup_file_name_to_modified_at_map.find(fname)!=full_backup_file_name_to_modified_at_map.end() && check_after_is_after(full_backup_file_name_to_modified_at_map[fname], curr_time)) || (full_backup_file_name_to_modified_at_map.find(fname)==full_backup_file_name_to_modified_at_map.end())){//this file has been modified
                            std::filesystem::copy_file(fpath, dest_fpath);
                            files.push_back(tuple<string, string, int> (fname, curr_time, backup_id));
                            bs.write_to_file(backup_id, backup_state_path.c_str(), files, "ongoing"); 
                        }
                        else {//file has not changed since the last full backup
                            files.push_back(tuple<string, string, int> (fname, curr_time, full_backup_file_name_to_backup_id_map[fname]));
                            bs.write_to_file(backup_id, backup_state_path.c_str(), files, "ongoing"); 
                        }
                        
                    }
                }
            }
            bs.write_to_file(backup_id, backup_state_path.c_str(), files, "completed"); 
            b_db->update_backup(backup_id, curr_time_string().c_str(), "completed");
            return 0;
        }

};