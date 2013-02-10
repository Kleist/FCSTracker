#include "colorfilter.h"

ColorFilter::ColorFilter(int hueMin, int hueMax, int& satMin, int& valMin)
  : hsvMin_(hueMin, 100, 100)
  , hsvMax_(hueMax, 255, 255)
  , hueMin_(hueMin)
  , satMin_(satMin)
  , valMin_(valMin)
{}

cv::Mat ColorFilter::filter(cv::Mat input) {
  cv::Mat hsvImg;
  cv::cvtColor(input, hsvImg, CV_BGR2HSV);
  cv::Mat mask;
  cv::inRange(hsvImg, cv::Scalar(hueMin_, satMin_, valMin_), hsvMax_, mask);
  cv::Mat result;
  input.copyTo(result, mask);
  return  result;
}
