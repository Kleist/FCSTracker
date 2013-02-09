#include <opencv2/opencv.hpp>

#include "processingchain.h"

bool pressedESC() {
    static const int EscapeKey = 27;
    return cv::waitKey(30) == EscapeKey;
}

int main() {
  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "Couldn't open videocam" << std::endl;
    return -1;
  }
  
  int showLevel = 0;
  cv::namedWindow("edges", 1);

  ProcessingChain chain;
  chain.addStep([](cv::Mat frame) {
    cv::Mat result;
    cv::cvtColor(frame, result, CV_BGR2GRAY);
    return result;
  });

  cv::createTrackbar("level", "edges", &showLevel, chain.frameCount()-1);
  while (!pressedESC()) {
    cv::Mat frame;
    cap >> frame;
    chain.process(frame);
    cv::imshow("edges", chain.getResultAtStep(showLevel));
  }
}
