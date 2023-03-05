#include <fstream>
#include "json/json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map> 

using namespace std; 

class BackupState { 
    private:
        int backup_id; 
        vector<tuple<string, string, int> > files; //path of the file, last modified datetime, last_backup_id in which file was changed

    public: 
        BackupState(){}; 

        BackupState(const char* path){
            this->load_from_file(path);
        }

        BackupState(string path){
            this->load_from_file(path.c_str());
        }

        vector<tuple<string, string, int> > get_files(){
            return this->files;
        }

        vector<string> get_file_paths(){
            vector<string> res; 
            for (int i=0; i<files.size(); i++){
                res.push_back(get<0>(this->files[i]));
            }
            return res; 
        }

        map<string, string> get_file_path_to_datetime_map(){
            map<string, string> m; 
            for (int i=0; i<files.size(); i++){
                m[get<0>(this->files[i])] = get<1>(this->files[i]);
            }
            return m; 
        }

        map<string, int> get_file_path_to_backup_id_map(){
            map<string, int> m; 
            for (int i=0; i<files.size(); i++){
                m[get<0>(this->files[i])] = get<2>(this->files[i]);
            }
            return m; 
        }

        int load_from_file(const char* path){
            std::ifstream f(path);
            nlohmann::json data = nlohmann::json::parse(f);
            this->backup_id = data["backup_id"];
            cout<<data.dump()<<endl;
            for(auto &array : data["files"]) {
                string path = array[0];
                string last_modified_dt = array[1];
                int last_backup_id = array[2];
                this->files.push_back(tuple<string, string, int> (path, last_modified_dt, last_backup_id));
            }
            return 0;
        }

        int write_to_file(int backup_id, const char* path, vector<tuple<string, string, int> >& files, string status){
            nlohmann::json data ;
            data["backup_id"]=backup_id;
            data["files"]=files;
            data["status"]=status; 
            ofstream o(path);
            o << setw(4) << data << endl;
            return 0;
        }  
};