#include "backgroundestimator.h"

static const double ForgettingFactor= 0.9;
static const int FrameMemory = 10;
static const int ChangeThreshold = 20;

void BackgroundEstimator::addFrame(cv::Mat newFrame) {
  if (oldest_ == -1) { // Init
    est_ = newFrame.clone();
    lastFrame_ = newFrame;
    frames_.resize(FrameMemory);
    std::fill(frames_.begin(), frames_.end(), newFrame.clone());
    next_();
  }
  else {
    cv::absdiff(lastFrame_, newFrame, frames_[oldest_]);
    next_();
    // Calculate diff with oldest frame
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
    // Create mask of diff small enough:
    cv::Mat mask;
    cv::inRange(absDiffSum, cv::Scalar(0,0,0), cv::Scalar(ChangeThreshold,ChangeThreshold,ChangeThreshold), mask);
    newFrame.copyTo(est_,mask);
    // Where diff is small enough, update background estimate for those pixels
    //est_ = (1-ForgettingFactor)*frame + ForgettingFactor * est_ - cv::Scalar(1,1,1);
    lastFrame_ = newFrame.clone();
  }
}

void BackgroundEstimator::next_() {
  oldest_ = (oldest_+1)%FrameMemory;
}

cv::Mat BackgroundEstimator::backgroundEstimate() {
  return est_;
}

