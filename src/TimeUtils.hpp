#ifndef CHRONO
#define CHRONO
#include <chrono>    
#endif

#ifndef FILESYSTEM
#define FILESYSTEM
#include <filesystem>
#endif

const char* curr_time_string();
const char* file_time_to_time_string(const std::filesystem::file_time_type& time);