#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

class StopWatch {
  typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
  TimePoint start_;
public:
  StopWatch() {
    start_ = std::chrono::system_clock::now();
  }

  long long getMillisAndReset() {
    using namespace std::chrono;
    TimePoint end = system_clock::now();
    milliseconds diff = duration_cast<milliseconds>(end-start_);
    start_ = end;
    return diff.count();
  }
};

#endif // STOPWATCH_H
