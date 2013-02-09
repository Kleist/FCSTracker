#ifndef PROCESSINGCHAIN_H
#define PROCESSINGCHAIN_H

#include "opencv2/opencv.hpp"

class ProcessingChain {
  typedef std::function<cv::Mat(cv::Mat)> ProcessStep;
  std::vector<cv::Mat> frames_;
  std::vector<ProcessStep> steps_;
public:
  void addStep(ProcessStep step) {
    steps_.push_back(step);
  }

  void process(cv::Mat frame) {
    frames_.clear();
    frames_.push_back(frame);
    for(ProcessStep step : steps_) {
      frames_.push_back(step(frames_.back()));
    }
  }

  size_t frameCount() const {
    return steps_.size()+1; // Includes original image
  }

  const cv::Mat& getResultAtStep(size_t step) {
    assert(step < frames_.size());
    return frames_[step];
  }
};

#endif // PROCESSINGCHAIN_H
