#ifndef PROCESSINGCHAIN_H
#define PROCESSINGCHAIN_H

#include "opencv2/opencv.hpp"

class ProcessingChain {
  typedef std::function<void(const cv::Mat&,cv::Mat&)> ProcessStep;
  std::vector<cv::Mat> frames_;
  std::vector<ProcessStep> steps_;

public:
  void addStep(ProcessStep step);
  size_t frameCount() const;
  const cv::Mat& processUntilStep(size_t step, cv::Mat frame);

private:
  void process_(cv::Mat frame, size_t steps);
};

#endif // PROCESSINGCHAIN_H
