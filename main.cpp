#include <memory>

#include <opencv2/opencv.hpp>

#include "stopwatch.h"
#include "backgroundhider.h"
#include "colorfilter.h"

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
  int threshold = 30;
  int satMin = 100;
  int valMin = 100;
  cv::createTrackbar("threshold", TrackerName, &threshold, 255);
  cv::createTrackbar("satMin", TrackerName, &satMin, 255);
  cv::createTrackbar("valMin", TrackerName, &valMin, 255);
  BackgroundHider bgHider(threshold);
  ColorFilter redFilter(0, 10, satMin, valMin);
  StopWatch stopWatch;
  return processFrames([&](cv::Mat frame) {
    cv::Mat withoutBackground = bgHider.process(frame);
    cv::Mat processed = redFilter.filter(withoutBackground);
    //cv::Mat processed = redFilter.filter(withoutBackground);
    std::ostringstream oss;
    auto millis = stopWatch.getMillisAndReset();
    oss << 1000/millis;
    cv::putText(processed, oss.str(),cv::Point(30,frame.rows-30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200,200,250),1,CV_AA);
    cv::imshow(BGName, bgHider.backgroundEstimate());
    cv::imshow(TrackerName, processed);
  });
}
