#include <memory>
#include <chrono>
#include <iomanip>
#include <ios>

#include <opencv2/opencv.hpp>

#include "backgroundestimator.h"

bool pressedESC() {
    static const int EscapeKey = 27;
    return cv::waitKey(1) == EscapeKey;
}

int processFrames(std::function<void(cv::Mat)> body) {
  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "Couldn't open videocam" << std::endl;
    return -1;
  }

  while (!pressedESC()) {
    cv::Mat frame;
    cap >> frame;
    body(frame);
  }
}

const char*const BGName = "Background Estimate";
const char*const TrackerName = "FCS Tracker";

int main() {
  cv::namedWindow(TrackerName);
  cv::namedWindow(BGName);
  BackgroundEstimator bgEst;
  return processFrames([&](cv::Mat frame) {
    bgEst.addFrame(frame);
    cv::imshow(BGName, bgEst.backgroundEstimate());
    cv::imshow(TrackerName, frame);
  });
}
