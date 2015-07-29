#include "Timer.h"

namespace tsgl {

Timer::Timer(double period) {
    reset(period);
}

Timer::~Timer() {
}

// Get the number of reps since start
unsigned int Timer::getReps() const {
    return std::chrono::duration_cast<duration_d>(highResClock::now() - start_time).count() / period_.count();
}

// Get the time since start
double Timer::getTime() const {
    return std::chrono::duration_cast<duration_d>(highResClock::now() - start_time).count();
}

// Get the time between the two last sleeps
double Timer::getTimeBetweenSleeps() const {
    return time_between_sleeps;
}

// Check if the timer has elapsed past the point when it last past the period
bool Timer::pastPeriod() {
    if (last_rep < getReps()) {
        last_rep = getReps();
        return true;
    }
    return false;
}

// Changes the period to the given interval
void Timer::reset(double period) {
    start_time = last_time = highResClock::now();
    if (period > 0) period_ = duration_d(period);
    last_rep = 0;
}

// Sleep the thread until the period has passed
void Timer::sleep(bool update) {
    mutexLock sleepLock(sleep_);
    timepoint_d sleep_time = last_time;
    while (sleep_time < highResClock::now()) {
      sleep_time = sleep_time + period_;
    }
    sleepLock.unlock();

    if (update)
      last_time = sleep_time;

    long double nano = std::chrono::duration_cast<duration_d>(sleep_time - highResClock::now()).count();
    std::this_thread::sleep_for(std::chrono::nanoseconds((long long) (nano * 1000000000)));

    time_between_sleeps = std::chrono::duration_cast<duration_d>(highResClock::now() - sleep_time).count()
        + period_.count();
}

// Sleep the thread for a specified duration
void Timer::threadSleepFor(double duration) {
    std::this_thread::sleep_for(std::chrono::nanoseconds((long long) (duration * 1000000000)));
}

}
