#include <string>
using namespace std; 

class Backup { 
    private:
        int id; 
        int backup_config_id; 
        char const* start_dt; 
        char const* end_dt; 
        char const* path; 
        char const* status; 

    public: 
        Backup(int id, int backup_config_id, char const* start_dt, char const* end_dt, char const* path, char const* status){
            this->id = id; 
            this->backup_config_id = backup_config_id;
            this->start_dt = start_dt;
            this->end_dt = end_dt;
            this->path = path;
            this->status = status;
        }

        void display(){
            printf("id: %d, backup_config_id: %d, start_dt: %s, end_dt: %s, path: %s, status: %s\n",  this->id, this->backup_config_id, this->start_dt, this->end_dt, this->path, this->status);
        }
};