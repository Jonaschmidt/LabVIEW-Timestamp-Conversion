/* TODO: 

    - C++20 compilation ?
*/
#include <iostream> // this can probably be removed before final release
#include <fstream>  // this can probably be removed before final release

#include <cstdint>
#include <cmath>
#include <chrono>

extern "C"{

// define the LabVIEW timestamp struct (64-bit seconds, 64-bit fraction)
struct lvTimestamp
{
    uint64_t fraction;   // fractional seconds (unsigned 64-bit)
    int64_t seconds;     // seconds since 01/01/1904 00:00:00 (signed 64-bit)
};

// TODO: factor in timezones
// seems to work for times even before UNIX epoch
// seems to work for times even before LabVIEW epoch
__declspec(dllexport)
void LogTimestampValuesReadableWChrono(const lvTimestamp* timestamp){
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
    //logFile << add_seconds << std::endl;
    //logFile << add_milliseconds << std::endl;
    logFile << "---" << std::endl;
}

} // extern "C"

/*
// (for testing functions)
int main()
{
    // seconds: 3037456923, Fraction: 8411715297611555536 (04/01/2000 18:02:03.456)
    lvTimestamp labviewTimestamp = {8411715297611555536, 3037456923};
    lvTimestamp* labviewTimestampPtr = &labviewTimestamp;

    // log the timestamp values into a file
    //LogTimestampValues(labviewTimestampPtr);

    // convert the timestamp to a UNIX timestamp, then as a human-readable string
    // LogTimestampValuesReadable(labviewTimestampPtr);
    LogTimestampValuesReadableWChrono(labviewTimestampPtr);

    // seconds: 7862400, Fraction: 8411715297611555536 (04/01/1904 00:00:00.456)
    labviewTimestamp = {8411715297611555536, 7862400};
    labviewTimestampPtr = &labviewTimestamp;
    LogTimestampValuesReadableWChrono(labviewTimestampPtr);

    // seconds: -60, Fraction: 8411715297611555536 (12/31/1903 23:59:00.456)
    labviewTimestamp = {8411715297611555536, -60};
    labviewTimestampPtr = &labviewTimestamp;
    LogTimestampValuesReadableWChrono(labviewTimestampPtr);

    return 0;
}
*/


