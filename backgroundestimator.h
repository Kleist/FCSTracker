#ifndef BACKGROUNDESTIMATOR_H
#define BACKGROUNDESTIMATOR_H

#include "opencv2/opencv.hpp"

class BackgroundEstimator {
  cv::Mat frame_;
public:
  void addFrame(cv::Mat frame);
  cv::Mat backgroundEstimate();
};

#endif // BACKGROUNDESTIMATOR_H
