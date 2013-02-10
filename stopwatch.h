#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

class StopWatch {
  typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
  TimePoint start_;
public:
  StopWatch();
  long long getMillisAndReset();
};

#endif // STOPWATCH_H
