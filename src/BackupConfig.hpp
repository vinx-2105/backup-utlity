#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef STAT
#define STAT
#include <sys/stat.h>
#endif

using namespace std; 

class BackupConfig { 
    private:
        int id; 
        string src_path; 
        string dest_path; 
        string mode; 
        string base_dt; 
        int full_frequency; 
        int part_frequency;
        string deleted_at; 

    public: 
        BackupConfig(int id, string src_path, string dest_path, string mode, string base_dt, int full_frequency, int part_frequency, string deleted_at){
            this->id = id; 
            this->src_path = src_path;
            this->dest_path = dest_path;
            this->mode = mode;
            this->base_dt = base_dt;
            this->full_frequency = full_frequency;
            this->part_frequency = part_frequency;
            this->deleted_at = deleted_at;
        }

        void display(){
            printf("id: %d, src_path: %s, dest_path: %s, mode: %s, base_dt: %s, full_frequency: %d, part_frequency: %d\n",  this->id,this->src_path.c_str(), this->dest_path.c_str(), this->mode.c_str(), this->base_dt.c_str(), this->full_frequency, this->part_frequency);
        }

        bool validate_src(){
            printf("Backup_config::validating src_path: %s\n", this->src_path.c_str());
            struct stat sb;//TODO: why do we need to user the struct qualifier
            return stat(this->src_path.c_str(), &sb)==0;
        }

        bool validate_dest(){
            printf("Backup_config::validating dest_path: %s\n", this->dest_path.c_str());
            struct stat sb;
            return (stat(this->dest_path.c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR)); 
        }

        const char* get_src_path(){
            return this->src_path.c_str();
        }

        const char* get_dest_path(){
            return this->dest_path.c_str();
        }
};