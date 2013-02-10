#include <memory>

#include <opencv2/opencv.hpp>

#include "stopwatch.h"
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
  return 0;
}

const char*const BGName = "Background Estimate";
const char*const TrackerName = "FCS Tracker";

int main() {
  cv::namedWindow(TrackerName);
  cv::namedWindow(BGName);
  BackgroundEstimator bgEst;
  StopWatch stopWatch;
  return processFrames([&](cv::Mat frame) {
    auto millis = stopWatch.getMillisAndReset();
    bgEst.addFrame(frame);
    cv::imshow(BGName, bgEst.backgroundEstimate());
    std::ostringstream oss;
    oss << 1000/millis;
    cv::putText(frame, oss.str(),cv::Point(30,frame.rows-30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200,200,250),1,CV_AA);
    cv::imshow(TrackerName, frame);
  });
}
