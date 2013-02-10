#include "backgroundhider.h"

BackgroundHider::BackgroundHider(int& threshold)
  : threshold_(threshold)
{
}

cv::Mat BackgroundHider::process(cv::Mat frame, cv::Mat backgroundEstimate) {
  cv::Mat diff;
  cv::absdiff(frame, backgroundEstimate, diff);
  cv::Mat mask;
  cv::inRange(diff, cv::Scalar(threshold_, threshold_, threshold_), cv::Scalar(255, 255, 255), mask);
  cv::Mat output;
  frame.copyTo(output, mask);
  return output;
}
