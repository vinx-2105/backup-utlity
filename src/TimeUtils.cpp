#include "TimeUtils.hpp"

using namespace std::chrono;

std::string time_t_to_datetime_string(std::time_t t){
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %X");
    std::string s = ss.str().c_str();
    return s;
}

std::string curr_time_string(){
    time_point<system_clock> now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    return time_t_to_datetime_string(now_time);
}

std::string file_time_to_time_string(const std::filesystem::file_time_type& time){
    std::time_t t = std::filesystem::_FilesystemClock::to_time_t(time);
    return time_t_to_datetime_string(t);
}

int datetime_str_to_epoch(std::string datetime){ //2023-03-05 19:12:18
    std::tm t = {};
    std::stringstream ss(datetime);
    std::time_t time;

    if (ss >> std::get_time(&t, "%Y-%m-%d %X")){
        std::put_time(&t, "%c");
        time = std::mktime(&t);
        // printf("timenumber:::::%ld\n", time);
    }
    else{
        printf("Parse failed for datetime \n");
    }
    return time; 
}

bool check_after_is_after(std::string& before_datetime, std::string& after_datetime){
    return datetime_str_to_epoch(after_datetime) > datetime_str_to_epoch(before_datetime);
}

bool time_sort_comparator(std::string a, std::string b){
    return datetime_str_to_epoch(a) < datetime_str_to_epoch(b);
}