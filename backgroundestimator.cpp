#include "backgroundestimator.h"

static const double ForgettingFactor= 0.9;
static const int FrameMemory = 10;

BackgroundEstimator::BackgroundEstimator(int& threshold)
  : threshold_(threshold)
{
}

void BackgroundEstimator::addFrame(cv::Mat newFrame) {
  if (oldest_ == -1) { // Init
    newFrame.convertTo(est_, CV_32F, 1);
    cv::accumulateWeighted(newFrame, est_, 1.0);
    lastFrame_ = newFrame;
    frames_.resize(FrameMemory);
    std::fill(frames_.begin(), frames_.end(), newFrame.clone());
    next_();
  }
  else {
    cv::absdiff(lastFrame_, newFrame, frames_[oldest_]);
    next_();

    cv::Mat absDiffSum;
    bool first = true;
    for (cv::Mat frame : frames_) {
        if (first) {
          absDiffSum = frame.clone();
        }
        else {
          absDiffSum += frame;
        }
    }

    cv::Mat mask;
    cv::inRange(absDiffSum, cv::Scalar(0,0,0), cv::Scalar(threshold_, threshold_, threshold_), mask);
    cv::accumulateWeighted(newFrame, est_, 0.05, mask);
    lastFrame_ = newFrame.clone();
  }
}

void BackgroundEstimator::next_() {
  oldest_ = (oldest_+1)%FrameMemory;
}

cv::Mat BackgroundEstimator::backgroundEstimate() {
  cv::Mat ret;
  est_.convertTo(ret, CV_8U);
  return ret;
}

