#include <opencv2/opencv.hpp>

#include "processingchain.h"

bool pressedESC() {
    static const int EscapeKey = 27;
    return cv::waitKey(1) == EscapeKey;
}

void toGrayScale(const cv::Mat& frame, cv::Mat& output) {
  cv::cvtColor(frame, output, CV_BGR2GRAY);
}

void blurStep(const cv::Mat& frame, cv::Mat& output) {
  cv::GaussianBlur(frame, output, cv::Size(101,101), 10);
}

int main() {
  using namespace cv;
  VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "Couldn't open videocam" << std::endl;
    return -1;
  }
  
  Mat background;
  cap >> background;
  ProcessingChain chain;
  chain.addStep(toGrayScale);
  chain.addStep(blurStep);
//  chain.addStep([background](Mat frame) {
//    Mat mask;
//    Mat diff = frame-background;
//    inRange(diff, Scalar(30, 30, 30), Scalar(255,255,255), mask);
//    return diff;
//  });

  int showLevel = 0;
  namedWindow("edges", cv::WINDOW_AUTOSIZE);
  createTrackbar("level", "edges", &showLevel, chain.frameCount()-1);
  while (!pressedESC()) {
    Mat frame;
    cap >> frame;
    imshow("edges", chain.processUntilStep(showLevel, frame));
  }
}
