#include <fstream>
#include <sstream>
#include <iomanip>

#include "TimeStamp.h"





namespace Time
{
    std::string getTimestamp()
    {
        std::time_t t = std::time(nullptr);
        std::tm tm{};


        //just in case u gonna run it on somehting else :P
        #ifdef _WIN32
        localtime_s(&tm, &t);
        #else
        localtime_r(&t, &tm);
        #endif


        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
        return oss.str();
    }
}


