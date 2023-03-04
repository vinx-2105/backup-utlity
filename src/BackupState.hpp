#include <fstream>
#include "json/json.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace std; 

class BackupState { 
    private:
        int backup_id; 
        vector<tuple<const char*, const char*, int> > files; //path of the file, last modified datetime, last_backup_id in which file was changed

    public: 
        int load_from_file(const char* path){
            std::ifstream f(path);
            nlohmann::json data = nlohmann::json::parse(f);
            this->backup_id = data["backup_id"];
            cout<<data.dump()<<endl;
            for(auto &array : data["files"]) {
                string path = array[0];
                string last_modified_dt = array[1];
                int last_backup_id = array[2];
                this->files.push_back(tuple<const char*, const char*, int> (path.c_str(), last_modified_dt.c_str(), last_backup_id));
            }
            return 0;
        }

        int save_new(int backup_id, const char* path, vector<tuple<const char*, const char*, int> >& files){
            printf("save_new:::backup_id%d\n", backup_id);
            printf("save_new:::path%s\n", path);
            printf("files[0]:::files[0]%s\n", );
            nlohmann::json data ;
            data["backup_id"]=backup_id;
            data["files"]=files;
            ofstream o(path);
            o << setw(4) << data << endl;
            return 0;
        }        
};