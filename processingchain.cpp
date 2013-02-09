#include "processingchain.h"

void ProcessingChain::addStep(ProcessStep step) {
  steps_.push_back(step);
}

size_t ProcessingChain::frameCount() const {
  return steps_.size()+1; // Includes original image
}

const cv::Mat& ProcessingChain::processUntilStep(size_t step, cv::Mat frame) {
  process_(frame, step);
  return frames_[step];
}

void ProcessingChain::process_(cv::Mat frame, size_t steps) {
  frames_.resize(steps_.size()+1);
  frames_[0] = frame;
  if (steps>0) {
    for (size_t step = 0; step<steps; ++step) {
      steps_[step](frames_[step], frames_[step+1]);
    }
  }
}

