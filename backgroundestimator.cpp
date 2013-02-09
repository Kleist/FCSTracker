#include "backgroundestimator.h"

void BackgroundEstimator::addFrame(cv::Mat frame) {
  using namespace cv;
  frame_ = frame.clone();
  circle(frame_, Point(0,0), 100, Scalar(0,255,0));
  circle(frame_, Point(frame_.cols,frame_.rows), 100, Scalar(0,255,0));
}

cv::Mat BackgroundEstimator::backgroundEstimate() {
  return frame_;
}

