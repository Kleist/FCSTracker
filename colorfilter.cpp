#include "colorfilter.h"

ColorFilter::ColorFilter(int hueMin, int hueMax)
  : hsvMin_(hueMin, 100, 100)
  , hsvMax_(hueMax, 255, 255)
{}

cv::Mat ColorFilter::filter(cv::Mat input) {
  cv::Mat hsvImg;
  cv::cvtColor(input, hsvImg, CV_BGR2HSV);
  cv::Mat mask;
  cv::inRange(hsvImg, hsvMin_, hsvMax_, mask);
  cv::Mat result;
  input.copyTo(result, mask);
  return  result;
}
