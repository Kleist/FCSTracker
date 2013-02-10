#include "stopwatch.h"

StopWatch::StopWatch() {
  start_ = std::chrono::system_clock::now();
}

long long StopWatch::getMillisAndReset() {
  using namespace std::chrono;
  TimePoint end = system_clock::now();
  milliseconds diff = duration_cast<milliseconds>(end-start_);
  start_ = end;
  return diff.count();
}

