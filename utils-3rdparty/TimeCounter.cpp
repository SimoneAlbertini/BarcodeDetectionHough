
#include "TimeCounter.hpp"

using namespace boost::chrono;

namespace artelab
{

    TimeCounter::TimeCounter(const TimeCounter& orig) 
    {
        *this = orig;
    }

    TimeCounter& TimeCounter::operator =(const TimeCounter& other)
    {
        this->_start = other._start;
        this->_end = other._end;
        return *this;
    }

    void TimeCounter::start()
    {
        _start = thread_clock::now();
    }

    void TimeCounter::stop()
    {
        _end = thread_clock::now();
    }

    long TimeCounter::get_milliseconds()
    {
        return duration_cast<milliseconds>(_end - _start).count();
    }

    long TimeCounter::get_nanoseconds()
    {
        return duration_cast<nanoseconds>(_end - _start).count();
    }

    double TimeCounter::get_seconds()
    {
        return (double) get_milliseconds() / 1000.0;
    }

}