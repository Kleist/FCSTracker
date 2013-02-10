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

int main(int argc, char** argv) {
  if (argc>1 && argv[1] == std::string("--record")) {
    std::string filename = "input.avi";
    if (argc>2) filename = argv[2];

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
      std::cerr << "Couldn't open videocam" << std::endl;
      return -1;
    }

    cv::Mat frame;
    cap >> frame;

    cv::VideoWriter writer;
    writer.open(filename,  CV_FOURCC('D','I','V','X'),30, frame.size(), true);
    if (!writer.isOpened()) {
        std::cerr << "Couldn't open writer with file: " << filename << "\n";
        return -2;
    }

    cv::namedWindow("input");
    while (cv::waitKey(1)<1) {
      cap >> frame;
      cv::imshow("input", frame);
      writer << frame;
    }
    return 0;
  }
  cv::namedWindow(BGName);
  cv::namedWindow(TrackerName);
  int threshold = 30;
  int satMin = 100;
  int valMin = 100;
  int showStage = 0;
  cv::createTrackbar("threshold", TrackerName, &threshold, 255);
  cv::createTrackbar("showStage", TrackerName, &showStage, 5);
  BackgroundHider bgHider(threshold);
  ColorFilter redFilter(0, 10, satMin, valMin);
  StopWatch stopWatch;
  return processFrames([&](cv::Mat frame) {
    cv::Mat withoutBackground = bgHider.process(frame);
    cv::Mat eroded;
    cv::erode(withoutBackground, eroded, cv::Mat(), cv::Point(-1,-1), 3);
    cv::Mat dilated;
    cv::dilate(eroded, dilated, cv::Mat(), cv::Point(-1,-1), 5);
    //cv::Mat processed = redFilter.filter(withoutBackground);
    cv::Mat* showImage = 0;
    switch (showStage) {
      case 0: showImage = &frame; break;
      case 1: showImage = &withoutBackground; break;
      case 2: showImage = &eroded; break;
      case 3: showImage = &dilated; break;
      default: showImage = 0;
    }
    if (showImage) {
      std::ostringstream oss;
      auto millis = stopWatch.getMillisAndReset();
      oss << 1000/millis;
      cv::putText(*showImage, oss.str(),cv::Point(30,frame.rows-30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200,200,250),1,CV_AA);
      cv::imshow(BGName, bgHider.backgroundEstimate());
      cv::imshow(TrackerName, *showImage);
    }
  });
}
