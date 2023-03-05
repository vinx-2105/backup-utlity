#ifndef CHRONO
#define CHRONO
#include <chrono>    
#endif

#ifndef CTIME
#define CTIME
#include <ctime>
#endif

#ifndef SSTREAM
#define SSTREAM
#include <sstream>
#endif

#ifndef IOMANIP
#define IOMANIP 
#include <iomanip>
#endif

#ifndef FILESYSTEM
#define FILESYSTEM
#include <filesystem>
#endif

#ifndef STRING
#define STRING
#include <string>
#endif

std::string time_t_to_datetime_string(std::time_t t);
std::string curr_time_string();
std::string file_time_to_time_string(const std::filesystem::file_time_type& time);
int datetime_str_to_epoch(std::string datetime);
bool check_after_is_after(std::string& before_datetime, std::string& after_datetime);
bool time_sort_comparator(std::string& a, std::string& b);