/* 
 * File:   TimeCounter.h
 * Author: simone
 *
 * Created on 23 maggio 2013, 10.38
 */

#ifndef TIMECOUNTER_H
#define	TIMECOUNTER_H

#include <boost/chrono/thread_clock.hpp>

class TimeCounter {
public:
    TimeCounter() {}
    TimeCounter(const TimeCounter& orig);
    virtual ~TimeCounter() {}
    
    TimeCounter& operator=(const TimeCounter& other);
    
    void start();
    void stop();
    long get_milliseconds();
    long get_nanoseconds();
    double get_seconds();
    
private:
    boost::chrono::thread_clock::time_point _start;
    boost::chrono::thread_clock::time_point _end;
};

#endif	/* TIMECOUNTER_H */

