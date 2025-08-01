#include <cstdint>
#include <cmath>
#include <chrono>

extern "C"{

// define the LabVIEW timestamp struct (64-bit fraction, 64-bit seconds)
struct lvTimestamp
{
    uint64_t fraction;   // fractional seconds (unsigned 64-bit)
    int64_t seconds;     // seconds since 01/01/1904 00:00:00 (signed 64-bit)
};

// TODO: factor in timezones ?
// seems to work for times even before UNIX epoch
// seems to work for times even before LabVIEW epoch
__declspec(dllexport)
void ConvertToChronoTS(const lvTimestamp* timestamp){
    using namespace std::chrono;

    const year_month_day LV_EPOCH = {January/01/1904};
    const uint64_t TWO_POW_64 = ~0ULL;  // maximum unit_64 value

    int64_t add_seconds = timestamp->seconds;

    // get milliseconds
    double fractionalSeconds = static_cast<double>(timestamp->fraction) / TWO_POW_64;
    uint16_t add_milliseconds = std::round(fractionalSeconds * 1000);

    sys_time<milliseconds> chronoTimestamp = sys_days(LV_EPOCH) + seconds(add_seconds);
    chronoTimestamp = chronoTimestamp + milliseconds(add_milliseconds);
}

} // extern "C"

