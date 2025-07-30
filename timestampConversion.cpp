/* TODO: 
    - implement the chrono library in place of time EVERYWHERE
    - enforce C++20 compilation ?
*/
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <time.h>

#include <chrono>

// define the LabVIEW timestamp struct (64-bit seconds, 64-bit fraction)
struct lvTimestamp
{
    uint64_t fraction;   // fractional seconds (unsigned 64-bit)
    int64_t seconds;     // seconds since 01/01/1904 00:00:00 (signed 64-bit)
};

// TODO: implementation
struct dtTimestamp{
    time_t time;
    uint64_t fracSecs;  // fractionsl seconds for extended accuracy
};

// function to log the timestamp values into a text file
extern "C" __declspec(dllexport) void LogTimestampValues(const lvTimestamp* timestamp)
{
    // open a text file for logging
    std::ofstream logFile("C:/Logs/log.txt", std::ios::app);  // open in append mode
    
    if (!logFile)
    {
        std::cerr << "Error opening file for logging!" << std::endl;
        return;
    }

    // log the seconds and fraction values
    logFile << "Fraction: " << timestamp->fraction << std::endl;
    logFile << "Seconds: " << timestamp->seconds << std::endl;

    // close the file
    logFile.close();
}

// TODO: try/catch on dates before the UNIX epoch
// TODO: include fractional seconds
// TODO: encorporate dtTimestamp struct
extern "C" __declspec(dllexport) void LogTimestampValuesReadable(const lvTimestamp* timestamp)
{
    const int LV_EPOCH_DIFF = 2082844800;
    const uint64_t TWO_POW_64 = ~0ULL;  // maximum unit_64 value

    int add_seconds = timestamp->seconds;
    //int64_t add_fracion = timestamp->fraction;

    //time_t UNIXts = (add_seconds + lvEpochDiff);
    time_t UNIXts = (time_t)(add_seconds - LV_EPOCH_DIFF);

    // get milliseconds
    double fractionalSeconds = static_cast<double>(timestamp->fraction) / TWO_POW_64;
    uint16_t milliseconds = std::round(fractionalSeconds * 1000);

    // open a text file for logging
    std::ofstream logFile("C:/Logs/log.txt", std::ios::app);  // open in append mode
    
    if (!logFile)
    {
        std::cerr << "Error opening file for logging!" << std::endl;
        return;
    }

    // convert the timestamp to something readable
    char* timeStr = ctime(&UNIXts);

    if (timeStr != nullptr){
        logFile << timeStr << "and " << milliseconds << " milliseconds";
        logFile << add_seconds << std::endl;
        logFile << milliseconds << std::endl;
        logFile << "---" << std::endl;
    }
    else
        logFile << "[Invalid timestamp]" << std::endl;
}

extern "C" __declspec(dllexport) void LogTimestampValuesReadableWChrono(const lvTimestamp* timestamp)
{
    using namespace std::chrono;

    const year_month_day LV_EPOCH = {January/01/1904};
    const uint64_t TWO_POW_64 = ~0ULL;  // maximum unit_64 value

    int64_t add_seconds = timestamp->seconds;

    // get milliseconds
    double fractionalSeconds = static_cast<double>(timestamp->fraction) / TWO_POW_64;
    uint16_t add_milliseconds = std::round(fractionalSeconds * 1000);

    // open a text file for logging
    std::ofstream logFile("C:/Logs/log.txt", std::ios::app);  // open in append mode
    
    sys_time<milliseconds> log_time = sys_days(LV_EPOCH) + seconds(add_seconds);
    log_time = log_time + milliseconds(add_milliseconds);

    if (!logFile)
    {
        std::cerr << "Error opening file for logging!" << std::endl;
        return;
    }

    logFile << log_time << std::endl;
    logFile << add_seconds << std::endl;
    logFile << add_milliseconds << std::endl;
    logFile << "---" << std::endl;
}

// (for testing functions)
int main()
{
    // seconds: 3037456923, Fraction: 8411715297611555536 (04/01/2000 18:02:03.456)
    lvTimestamp labviewTimestamp = {8411715297611555536, 3037456923};
    lvTimestamp* labviewTimestampPtr = &labviewTimestamp;

    // log the timestamp values into a file
    //LogTimestampValues(labviewTimestampPtr);

    // convert the timestamp to a UNIX timestamp, then as a human-readable string
    LogTimestampValuesReadable(labviewTimestampPtr);
    LogTimestampValuesReadableWChrono(labviewTimestampPtr);

    return 0;
}

