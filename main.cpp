#include <opencv2/opencv.hpp>

class ProcessingChain {
  typedef std::function<cv::Mat(cv::Mat)> ProcessStep;
  std::vector<cv::Mat> frames_;
  std::vector<ProcessStep> steps_;
public:
  void addStep(ProcessStep step) {
    steps_.push_back(step);
  }
  
  void process(cv::Mat frame) {
    frames_.clear();
    frames_.push_back(frame);
    for(ProcessStep step : steps_) {
      frames_.push_back(step(frames_.back()));
    }
  }
  
  size_t frameCount() const {
    return steps_.size()+1; // Includes original image
  }
  
  const cv::Mat& getResultAtStep(size_t step) {
    assert(step < frames_.size());
    return frames_[step];
  }
};

static const int EscapeKey = 27;
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
  while (1) {
    cv::Mat frame;
    cap >> frame;
    chain.process(frame);
    cv::imshow("edges", chain.getResultAtStep(showLevel));
    int pressedKey = cv::waitKey(30);
    if (pressedKey == EscapeKey) break;
    if (pressedKey>=0) {
      std::cout << static_cast<int>(pressedKey) << " pressed: " << pressedKey << std::endl;
    }
  }
}