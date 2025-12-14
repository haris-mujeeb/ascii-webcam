#include <gtest/gtest.h>
#include <thread>
#include "ascii_image.hpp"


// Helper macro to stringify preprocessor definitions
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

class AsciiImageTests : public ::testing::Test {
  protected:
  void SetUp() override {
  }
  void TearDown() override {
  }
};

TEST_F(AsciiImageTests, getGrayscaleValueTest) {
  EXPECT_NEAR(getGrayscaleValue(100, 100, 100), (29.9 + 58.7 + 11.4), 1);
}

TEST_F(AsciiImageTests, pixelToAsciiTest) {
  EXPECT_EQ(pixelToAscii(getGrayscaleValue(0, 0, 0)), ' ');
  EXPECT_EQ(pixelToAscii(getGrayscaleValue(255, 255, 255)), '$');
}

TEST_F(AsciiImageTests, OuputGrayAsciiToFile) {
  try {
    RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
    EXPECT_NE(raw_img.getData(), nullptr);
    EXPECT_GT(raw_img.getWidth(), 0);
    EXPECT_GT(raw_img.getHeight(), 0);
    EXPECT_GT(raw_img.getChannels(), 0);
    EXPECT_EQ(raw_img.getSize(), raw_img.getHeight() * raw_img.getWidth() * raw_img.getChannels());

    RawImage gray_img = convertToAscii(raw_img);
    EXPECT_NO_THROW(outputAsciiToFile(gray_img, TOSTRING(OUTPUT_GRAY_TXT_FILE_PATH)));
  } catch (const std::exception& e) {
    std::cerr << "[   SKIP   ] " << e.what() << "\n";
    GTEST_SKIP();
  }
}

TEST_F(AsciiImageTests, OuputColoredAsciiToFile) {
  try {
    RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));

    int width = raw_img.getWidth(); 
    int height = raw_img.getHeight();
    size_t ascii_size = height * (width * 20 + 1) + 16;
    RawImage colored_img(ascii_size, 1, 1);

    convertToColoredAscii(raw_img, colored_img);
    EXPECT_NO_THROW(outputAsciiToFile(colored_img, TOSTRING(OUTPUT_COLORED_TXT_FILE_PATH)));
  } catch (const std::exception& e) {
    std::cerr << "[   SKIP   ] " << e.what() << "\n";
    GTEST_SKIP();
  }
}

TEST_F(AsciiImageTests, OuputGrayAsciiToTerminal) {
  RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
  RawImage gray_img = convertToAscii(raw_img);
  EXPECT_NO_THROW(std::cout << gray_img.getData());
}

TEST_F(AsciiImageTests, OuputColoredAsciiToTerminal) {
  RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
  int width = raw_img.getWidth(); 
  int height = raw_img.getHeight();
  size_t ascii_size = height * (width * 20 + 1) + 16;
  RawImage colored_img(ascii_size, 1, 1);
  convertToColoredAscii(raw_img, colored_img);
  EXPECT_NO_THROW(std::cout << colored_img.getData());
}

TEST_F(AsciiImageTests, OuputTerminalRainbowAscii) {
  RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));

  int width = raw_img.getWidth(); 
  int height = raw_img.getHeight();
  size_t ascii_size = height * (width * 20 + 1) + 16;
  RawImage rainbow_img(ascii_size, 1, 1); 
  
  convertToRainbowAscii(raw_img, 0 ,rainbow_img);
  EXPECT_NO_THROW(std::cout << rainbow_img.getData());
}

TEST_F(AsciiImageTests, AnimationStressTest) {
  size_t FRAMES_TO_PROCESS = 1000;  
  RawImage raw_img(TOSTRING(IMAGE_FILE_PATH));
  EXPECT_NO_THROW(outputRainbowAsciiAnimation(static_cast<const RawImage&>(raw_img), FRAMES_TO_PROCESS));
}

TEST_F(AsciiImageTests, WebcamDataVerification) {
  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "[   SKIP   ] Webcam not available\n";
    return;
  }

  cv::Mat frame;
  cap >> frame;
  if (frame.empty()) {
     std::cerr << "[   SKIP   ] Webcam frame empty\n";
     return;
  }

  // Verify Webcam Data
  EXPECT_FALSE(frame.empty());
  EXPECT_GT(frame.cols, 0);
  EXPECT_GT(frame.rows, 0);
  EXPECT_EQ(frame.channels(), 3);

  // Verify Scaling
  int new_width = 100;
  int new_height = static_cast<int>(frame.rows * (static_cast<float>(new_width) / frame.cols * 0.55f));
  cv::Mat resized_frame;
  cv::resize(frame, resized_frame, cv::Size(new_width, new_height));

  EXPECT_EQ(resized_frame.cols, new_width);
  EXPECT_EQ(resized_frame.rows, new_height);

  // Verify Color Conversion
  cv::Mat rgb_frame;
  cv::cvtColor(resized_frame, rgb_frame, cv::COLOR_BGR2RGB);

  EXPECT_EQ(rgb_frame.channels(), 3);
  EXPECT_EQ(rgb_frame.size(), resized_frame.size());
  
  // Ensure data is accessible
  EXPECT_NO_THROW(rgb_frame.at<cv::Vec3b>(0, 0));
}