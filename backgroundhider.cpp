#include "backgroundhider.h"

BackgroundHider::BackgroundHider(int& threshold)
  : threshold_(threshold)
  , bgEst_(threshold_)
{
}

cv::Mat BackgroundHider::process(cv::Mat frame) {
  bgEst_.addFrame(frame);
  cv::Mat temp;
  cv::absdiff(frame, bgEst_.backgroundEstimate(), temp);
  cv::inRange(temp, cv::Scalar(threshold_, threshold_, threshold_), cv::Scalar(255, 255, 255), temp);
  cv::Mat output;
  frame.copyTo(output, temp);
  return output;
}

cv::Mat BackgroundHider::backgroundEstimate() {
  return bgEst_.backgroundEstimate();
}
