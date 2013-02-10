#include "backgroundestimator.h"

static const double ForgettingFactor= 0.9;
static const int FrameMemory = 10;

BackgroundEstimator::BackgroundEstimator(int& threshold)
  : threshold_(threshold)
{
}

void BackgroundEstimator::addFrame(cv::Mat newFrame) {
  if (est_.empty()) {
    newFrame.convertTo(est_, CV_32F, 1);
    lastFrame_ = newFrame;
  }
  else {
    cv::Mat diff;
    cv::absdiff(lastFrame_, newFrame, diff);

    cv::Mat mask;
    cv::inRange(diff, cv::Scalar(0,0,0), cv::Scalar(threshold_, threshold_, threshold_), mask);
    cv::accumulateWeighted(newFrame, est_, 0.05, mask);
    lastFrame_ = newFrame.clone();
  }
}

cv::Mat BackgroundEstimator::backgroundEstimate() {
  cv::Mat ret;
  est_.convertTo(ret, CV_8U);
  return ret;
}

