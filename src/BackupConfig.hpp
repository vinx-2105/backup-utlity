#include <string>
using namespace std; 

class BackupConfig { 
    private:
        int id; 
        char const* src_path; 
        char const* dest_path; 
        char const* mode; 
        char const* base_dt; 
        int full_frequency; 
        int part_frequency;
        char const* deleted_at; 

    public: 
        BackupConfig(int id, char const* src_path, char const* dest_path, char const* mode, char const* base_dt, int full_frequency, int part_frequency, char const* deleted_at){
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
            printf("id: %d, src_path: %s, dest_path: %s, mode: %s, base_dt: %s, full_frequency: %d, part_frequency: %d\n",  this->id,this->src_path, this->dest_path, this->mode, this->base_dt, this->full_frequency, this->part_frequency);
        }
};