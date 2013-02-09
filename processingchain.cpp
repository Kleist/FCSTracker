#include "processingchain.h"

void ProcessingChain::addStep(std::string name, ProcessStep step) {
  steps_.push_back(std::make_pair(name, step));
}

std::string ProcessingChain::getStepName(size_t step) {
  if (step == 0) return "Original";
  else return steps_[step-1].first;
}

size_t ProcessingChain::frameCount() const {
  return steps_.size()+1; // Includes original image
}

const cv::Mat& ProcessingChain::processUntilStep(size_t step, cv::Mat frame) {
  process_(frame, frameCount()-1);
  return frames_[step];
}

void ProcessingChain::process_(cv::Mat frame, size_t steps) {
  frames_.resize(steps_.size()+1);
  frames_[0] = frame;
  if (steps>0) {
    for (size_t step = 0; step<steps; ++step) {
      steps_[step].second(frames_[step], frames_[step+1]);
    }
  }
}

