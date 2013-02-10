#ifndef BACKGROUNDESTIMATOR_H
#define BACKGROUNDESTIMATOR_H

#include "opencv2/opencv.hpp"

class BackgroundEstimator {
  cv::Mat est_;
  cv::Mat lastFrame_;
  int& threshold_;
public:
  BackgroundEstimator(int& threshold);
  void addFrame(cv::Mat frame);
  cv::Mat backgroundEstimate();
};

#endif // BACKGROUNDESTIMATOR_H
