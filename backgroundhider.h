#ifndef BACKGROUNDHIDER_H
#define BACKGROUNDHIDER_H

#include "opencv2/opencv.hpp"
#include "backgroundestimator.h"

class BackgroundHider {
  int& threshold_;
  BackgroundEstimator bgEst_;
public:
  BackgroundHider(int& threshold);
  cv::Mat process(cv::Mat frame);
  cv::Mat backgroundEstimate();
};

#endif // BACKGROUNDHIDER_H
