#ifndef BACKGROUNDESTIMATOR_H
#define BACKGROUNDESTIMATOR_H

#include "opencv2/opencv.hpp"

class BackgroundEstimator {
  cv::Mat est_;
  cv::Mat lastFrame_;
  std::vector<cv::Mat> frames_;
  int oldest_ = -1;
public:
  void addFrame(cv::Mat frame);
  cv::Mat backgroundEstimate();
private:
  void next_();
};

#endif // BACKGROUNDESTIMATOR_H
