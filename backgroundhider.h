#ifndef BACKGROUNDHIDER_H
#define BACKGROUNDHIDER_H

#include "opencv2/opencv.hpp"

class BackgroundHider {
  int& threshold_;
public:
  BackgroundHider(int& threshold);
  cv::Mat process(cv::Mat frame, cv::Mat backgroundEstimate);
};

#endif // BACKGROUNDHIDER_H
