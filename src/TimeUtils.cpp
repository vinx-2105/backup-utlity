#include "TimeUtils.hpp"

using namespace std::chrono;

const char* curr_time_string(){
    time_point<system_clock> start;
    start = system_clock::now();
    std::time_t start_time = system_clock::to_time_t(start);
    return strtok(ctime(&start_time), "\n");;;
}

const char* file_time_to_time_string(const std::filesystem::file_time_type& time){
     std::time_t t = std::filesystem::_FilesystemClock::to_time_t(time);
    return ctime(&t);
}