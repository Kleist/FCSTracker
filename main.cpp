#include <memory>
#include <chrono>
#include <iomanip>
#include <ios>

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
  cv::GaussianBlur(frame, output, cv::Size(5,5), 10);
}

typedef std::shared_ptr<cv::Mat> SPMat;

struct Subtract {
  cv::Mat frame_;
  Subtract(cv::Mat frame) : frame_(frame.clone()) {}
  void operator()(const cv::Mat& input, cv::Mat& output) {
    output = input - frame_;
  }
};

struct SubtractLive {
  SPMat frame_;
  SubtractLive(SPMat frame) : frame_(frame) {}
  void operator()(const cv::Mat& input, cv::Mat& output) {
    output = input - *frame_;
  }
};

struct Add {
  SPMat frame_;
  Add(SPMat frame) : frame_(frame) {}
  void operator()(const cv::Mat& input, cv::Mat& output) {
    output = input + *frame_;
  }
};

struct PutSaved {
  std::shared_ptr<cv::Mat> saved_;
  PutSaved(std::shared_ptr<cv::Mat> saved) : saved_(saved) {
  }
  void operator()(const cv::Mat& input, cv::Mat& output) {
    *saved_ = input.clone();
    output = input;
  }
};

struct PutConstant {
  cv::Mat target_;
  PutConstant(cv::Mat target) : target_(target.clone()) {}
  void operator()(const cv::Mat&, cv::Mat& output) {
    output = target_.clone();
  }
};

struct GetSaved {
  std::shared_ptr<cv::Mat> saved_;
  GetSaved(std::shared_ptr<cv::Mat> saved) : saved_(saved) {}
  void operator()(const cv::Mat&, cv::Mat& output) {
    output = saved_->clone();
  }
};

struct MakeForegroundMask {
  SPMat mask_;
  SPMat background_;
  MakeForegroundMask(SPMat mask, SPMat background) : mask_(mask), background_(background) {}
  void operator()(const cv::Mat& input, cv::Mat& output) {
    cv::Mat diff;
    cv::absdiff(*background_, input, diff);
    cv::inRange(diff, cv::Scalar(128, 128, 128), cv::Scalar(255,255,255), *mask_);
    output = input;
  }
};

const auto Abs = [](const cv::Mat& input, cv::Mat& output) {
    output = cv::abs(input);
};

class StopWatch {
  typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
  TimePoint start_;
public:
  StopWatch() {
    start_ = std::chrono::system_clock::now();
  }

  double getTimeAndReset() {
    using namespace std::chrono;
    TimePoint end = system_clock::now();
    milliseconds diff = duration_cast<milliseconds>(end-start_);
    start_ = end;
    return diff.count();
  }
};

int main() {
  using namespace cv;
  VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "Couldn't open videocam" << std::endl;
    return -1;
  }
  
  auto background = std::make_shared<Mat>();
  // let camera adjust to lighting
  waitKey(1000);
  cap >> *background;
  ProcessingChain chain;
  auto input = std::make_shared<cv::Mat>();
  auto lastInput = std::make_shared<cv::Mat>(background->clone());
  *input = *background;
#define ADD_STEP(CHAIN, STEP) (CHAIN).addStep(#STEP,STEP);
  ADD_STEP(chain, PutSaved(input));
  ADD_STEP(chain, SubtractLive(lastInput));
  ADD_STEP(chain, GetSaved(input));
  ADD_STEP(chain, PutSaved(lastInput));
  ADD_STEP(chain, Subtract(*background));
//  ADD_STEP(chain, AddConstant(background));
//  ADD_STEP(chain, SubtractConstant(background));
  ADD_STEP(chain, Abs);
  ADD_STEP(chain, toGrayScale);
  ADD_STEP(chain, blurStep);

  int showLevel = 0;
  namedWindow("edges");
  createTrackbar("level", "edges", &showLevel, chain.frameCount()-1);
  StopWatch stopWatch;
  while (!pressedESC()) {
    Mat frame;
    cap >> frame;
    //Mat frameToShow = chain.processUntilStep(showLevel, frame);
//    putText(frameToShow, chain.getStepName(showLevel), Point(0,0), FONT_HERSHEY_SIMPLEX, 5, Scalar(255,255,255));
    std::ostringstream oss;
    oss.precision(1);
    oss << std::setw(20) << std::fixed << 1000.0/stopWatch.getTimeAndReset();
    oss << " fps - " << chain.getStepName(showLevel);
    frame = chain.processUntilStep(showLevel, frame);
    putText(frame, oss.str(), cvPoint(30,30),
        FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
    imshow("edges", frame);
  }
}
