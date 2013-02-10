#ifndef COLORFILTER_H
#define COLORFILTER_H

#include "opencv2/opencv.hpp"

class ColorFilter {
  cv::Scalar hsvMin_;
  cv::Scalar hsvMax_;
  int hueMin_;
  int& satMin_;
  int& valMin_;

public:
  ColorFilter(int hueMin, int hueMax, int& satMin, int& valMin);
  cv::Mat filter(cv::Mat);
};

#endif // COLORFILTER_H
