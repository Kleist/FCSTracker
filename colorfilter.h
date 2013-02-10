#ifndef COLORFILTER_H
#define COLORFILTER_H

#include "opencv2/opencv.hpp"

class ColorFilter {
  cv::Scalar hsvMin_;
  cv::Scalar hsvMax_;
public:
  ColorFilter(int hueMin, int hueMax);
  cv::Mat filter(cv::Mat);
};

#endif // COLORFILTER_H
